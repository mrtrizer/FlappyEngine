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
            "    v8::Local<v8::Object> wrap(SafePtrBase& ptr);\n"
            "    v8::Local<v8::Function> createConstructor();\n"
            "} // V8%s\n"
            "} // flappy\n",

             className.c_str(),
             className.c_str());
    return std::string(output.data());

}

std::string generateWrapperCpp(std::string className, GeneratedMethods generatedMethods) {
    std::vector<char> output(50000);
    snprintf(output.data(), output.size(),
            "#include <algorithm>\n"
            "#include <V8JSUtils.h>\n"
            "#include <V8BasicTypeWrappers.h>\n"
            "#include <SharedPtrHolder.h>\n"
            "#include <SafePtrHolder.h>\n"
            "#include <%s.h>\n"
            "namespace flappy {\n"
            "using namespace v8;\n"
            "namespace V8%s {\n"
            "void setMethods(Local<Object> prototype, Local<External> jsPtr);\n"
            "\n"
            "%s"
            "\n"
            ""
            "void setMethods(Local<Object> prototype, Local<External> jsPtr) {\n"
            "%s"
            "}\n"
            "void setMethodTemplates(Local<ObjectTemplate> prototype, Local<External> jsPtr) {\n"
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
            "Local<Object> wrap(SafePtrBase& ptr) {\n"
            "    auto castedPtr = static_cast<SafePtr<%s>&>(ptr);\n"
            "    EscapableHandleScope handle_scope(Isolate::GetCurrent());\n"
            "    Local <Context> context = Local <Context>::New (Isolate::GetCurrent(), Isolate::GetCurrent()->GetCurrentContext());\n"
            "    Context::Scope contextScope (context);\n"
            "    Local<External> jsPtr = External::New(Isolate::GetCurrent(), new SafePtrHolder<%s>(castedPtr));\n"
            "    Local<FunctionTemplate> funcTemplate = FunctionTemplate::New(Isolate::GetCurrent());\n"
            "    Local<ObjectTemplate> prototype = funcTemplate->PrototypeTemplate();\n"
            "    setMethodTemplates(prototype, jsPtr);\n"
            "\n"
            "    Local<ObjectTemplate> componentTemplate = funcTemplate->InstanceTemplate();\n"
            "    Local<ObjectTemplate> templ = Local<ObjectTemplate>::New(Isolate::GetCurrent(), componentTemplate);\n"
            "    Local<Object> result = templ->NewInstance(Isolate::GetCurrent()->GetCurrentContext()).ToLocalChecked();\n"
            "    result->SetPrivate(currentContext(), toV8PrivateKey(\"cpp_ptr\"), jsPtr);\n"
            "    return handle_scope.Escape(result);\n"
            "}\n"
            "} // V8%s \n"
            "} // flappy \n",
            className.c_str(),
            className.c_str(),
            generatedMethods.methodBodies.c_str(),
            generatedMethods.methodRefs.c_str(),
            generatedMethods.methodTemplateRefs.c_str(),
            className.c_str(),
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

            m_initializerStream << "    wrapperMap.set<" << className << ", Wrapper>({\"" << className << "\", "
                                <<"V8" << className << "::wrap, V8" << className << "::createConstructor });\n";
            m_initializerHeadersStream << "#include \"wrappers/V8" << className << ".h\"\n";
            m_initializerHeadersStream << "#include <" << className << ".h>\n";

            std::cout << "class " << className << std::endl;

            auto generatedMethods = processMethods(classDecl, className);

            auto cppFileData = generateWrapperCpp(className, generatedMethods);
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
            "#include <V8JSUtils.h>\n"
            "%s\n"
            "namespace flappy {\n"
            "TypeMap<void, Wrapper> getV8Wrappers() {\n"
            "    TypeMap<void, Wrapper> wrapperMap;\n"
            "%s"
            "    return wrapperMap;\n"
            "}\n"
            "} // flappy\n"
             ,initializerHeadersStream.c_str()
             ,initializers.c_str());
    return std::string(output.data());
}

std::string generateInitializerHeader() {
    return std::string(
            "#pragma once\n"
            "#include <TypeMap.h>\n"
            "#include <V8JSUtils.h>\n"
            "namespace flappy {\n"
            "    TypeMap<void, Wrapper> getV8Wrappers();\n"
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
