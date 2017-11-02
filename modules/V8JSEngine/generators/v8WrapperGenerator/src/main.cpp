#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "llvm/Support/CommandLine.h"

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

#include "generate_method.h"

#include <cstdio>
#include <iostream>
#include <fstream>
#include <thread>
#include <stdexcept>
#include <unistd.h>
#include <sstream>
#include <unordered_set>

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace llvm;

std::string generateWrapperHeader(std::string className) {

    std::vector<char> output(5000);
    snprintf(output.data(), output.size(),
            "#pragma once\n"
            "#include <v8.h>\n"
            "namespace flappy {\n"
            "namespace V8%s {\n"
            "    void setPos(const v8::FunctionCallbackInfo<v8::Value>& info);\n"
            "    v8::Local<v8::Object> wrap(void* ptr);\n"
            "    v8::Local<v8::Function> createConstructor();\n"
            "} // V8%s\n"
            "} // flappy\n",

             className.c_str(),
             className.c_str());
    return std::string(output.data());

}

std::string generateWrapperCpp(std::string className, std::string methodBodies, std::string methodRefs) {
    std::vector<char> output(50000);
    snprintf(output.data(), output.size(),
            "#include <V8JSManager.h>\n"
            "#include <V8BasicTypeWrappers.h>\n"
            "#include <algorithm>\n"
            "#include <%s.h>\n"
            "namespace flappy {\n"
            "using namespace v8;\n"
            "namespace V8%s {\n"
            "template <typename T>\n"
            "void setMethods(Local<T> prototype, Local<External> jsPtr);\n"
            "\n"
            "%s"
            "\n"
            ""
            "template <typename T>\n"
            "void setMethods(Local<T> prototype, Local<External> jsPtr) {\n"
            "%s"
            "}\n"
            "\n"
            "Local<Function> createConstructor() {\n"
            "    EscapableHandleScope handle_scope(Isolate::GetCurrent());\n"
            "    Local <Context> context = Local <Context>::New (Isolate::GetCurrent(), Isolate::GetCurrent()->GetCurrentContext());\n"
            "    Context::Scope contextScope (context);\n"
            "    Local<FunctionTemplate> funcTemplate = FunctionTemplate::New(Isolate::GetCurrent(), method_constructor);\n"
            "    return handle_scope.Escape(funcTemplate->GetFunction());\n"
            "}\n"
            "\n"
            "Local<Object> wrap(void* ptr) {\n"
            "    auto castedPtr = static_cast<%s*>(ptr);\n"
            "    EscapableHandleScope handle_scope(Isolate::GetCurrent());\n"
            "    Local <Context> context = Local <Context>::New (Isolate::GetCurrent(), Isolate::GetCurrent()->GetCurrentContext());\n"
            "    Context::Scope contextScope (context);\n"
            "    Local<External> jsPtr = External::New(Isolate::GetCurrent(), castedPtr);\n"
            "    Local<FunctionTemplate> funcTemplate = FunctionTemplate::New(Isolate::GetCurrent());\n"
            "    Local<Template> prototype = funcTemplate->PrototypeTemplate();\n"
            "    setMethods(prototype, jsPtr);\n"
            "\n"
            "    Local<ObjectTemplate> componentTemplate = funcTemplate->InstanceTemplate();\n"
            "    componentTemplate->SetInternalFieldCount(1);\n"
            "    Local<ObjectTemplate> templ = Local<ObjectTemplate>::New(Isolate::GetCurrent(), componentTemplate);\n"
            "    Local<Object> result = templ->NewInstance(Isolate::GetCurrent()->GetCurrentContext()).ToLocalChecked();\n"
            "    result->SetInternalField(0, jsPtr);\n"
            "    return handle_scope.Escape(result);\n"
            "}\n"
            "} // V8%s \n"
            "} // flappy \n",
            className.c_str(),
            className.c_str(),
            methodBodies.c_str(),
            methodRefs.c_str(),
            className.c_str(),
            className.c_str());
    return std::string(output.data());
}

class ClassHandler : public MatchFinder::MatchCallback {
public :
    ClassHandler(std::string path)
        : m_path(path)
    {}

    void writeTextFile(std::string name, std::string data) {
        std::ofstream textFile;
        textFile.open(m_path + "/wrappers/" + name);
        textFile << data;
        textFile.close();
    }

    virtual void run(const MatchFinder::MatchResult &Result) {
        if (const CXXRecordDecl *classDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classes")) {
            auto className = classDecl->getNameAsString();
            if ((className == "Component") || (className== "Manager") || (className == "JSComponent"))
                return;
            if (m_wrappedClasses.find(className) != m_wrappedClasses.end())
                return;

            m_wrappedClasses.insert(className);

            auto headerFileData = generateWrapperHeader(className);
            auto headerFileName = std::string("V8") + className + ".h";
            writeTextFile(headerFileName, headerFileData);

            m_initializerStream << "wrapperMap[\"flappy::" << className << "]\"] = {\"" << className << "\", "
                                <<"V8" << className << "::wrap, V8" << className << "::createConstructor };\n";
            m_initializerHeadersStream << "#include \"wrappers/V8" << className << ".h\"\n";

            std::cout << "class " << className << std::endl;

            auto generatedMethods = processMethods(classDecl, className);

            auto cppFileData = generateWrapperCpp(className, generatedMethods.methodBodies, generatedMethods.methodRefs);
            auto cppFileName = std::string("V8") + className + ".cpp";
            writeTextFile(cppFileName, cppFileData);
        }
    }

    std::string initializersStr() {
        return m_initializerStream.str();
    }

    std::string initializerHeadersStream() {
        return m_initializerHeadersStream.str();
    }

private:
    std::string m_path;
    std::stringstream m_initializerStream;
    std::stringstream m_initializerHeadersStream;
    std::unordered_set<std::string> m_wrappedClasses;
};

std::string generateInitializerCpp(const std::string& initializers, const std::string& initializerHeadersStream) {
    std::vector<char> output(5000);
    snprintf(output.data(), output.size(),
            "#include \"WrapperInitializer.h\"\n"
            "#include <V8JSManager.h>\n"
            "%s\n"
            "namespace flappy {\n"
            "void initV8Wrappers() {\n"
                "%s"
            "}\n"
            "} // flappy\n"
             ,initializerHeadersStream.c_str()
             ,initializers.c_str());
    return std::string(output.data());
}

std::string generateInitializerHeader() {
    return std::string(
            "#pragma once\n"
            "namespace flappy {\n"
            "    void initV8Wrappers();\n"
            "} // flappy\n");
}

static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
static cl::extrahelp MoreHelp("\nMore help text...");
static cl::OptionCategory v8JSGeneratorOptions("My tool options");
static cl::opt<std::string> outputPath("output", cl::cat(v8JSGeneratorOptions));

int main(int argc, const char **argv) {
    CommonOptionsParser optionsParser(argc, argv, v8JSGeneratorOptions);

    if (outputPath.getValue() == "") {
        std::cout << "Set output dirrectory with --output parameter" << std::endl;
        return 1;
    }

    ClangTool tool(optionsParser.getCompilations(), optionsParser.getSourcePathList());

    MatchFinder finder;
    auto methodMatcher = cxxRecordDecl(isClass(), isDefinition(), matchesName(".*(Component|Manager)$")).bind("classes");
    ClassHandler printer(outputPath.getValue());
    finder.addMatcher(methodMatcher, &printer);

    auto result = tool.run(newFrontendActionFactory(&finder).get());

    std::ofstream wrapperInitializerCpp;
    wrapperInitializerCpp.open(outputPath.getValue() + "/WrapperInitializer.cpp");
    wrapperInitializerCpp << generateInitializerCpp(printer.initializersStr(), printer.initializerHeadersStream());
    wrapperInitializerCpp.close();

    std::ofstream wrapperInitializerHeader;
    wrapperInitializerHeader.open(outputPath.getValue() + "/WrapperInitializer.h");
    wrapperInitializerHeader << generateInitializerHeader();
    wrapperInitializerHeader.close();

    return result;
}
