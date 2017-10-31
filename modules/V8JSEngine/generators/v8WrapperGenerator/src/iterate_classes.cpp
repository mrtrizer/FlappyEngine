// Declares clang::SyntaxOnlyAction.
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/ASTMatchers/ASTMatchers.h"
// Declares llvm::cl::extrahelp.
#include "llvm/Support/CommandLine.h"

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

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

auto methodMatcher = cxxRecordDecl(isClass(), isDefinition(), hasName("TransformComponent")).bind("classes");

std::string generateWrapperHeader(std::string className) {

    std::vector<char> output(5000);
    snprintf(output.data(), output.size(),
            "#pragma once\n"

            "#include <v8.h>\n"

            "namespace flappy {\n"

            "namespace V8%s {\n"

            "    void setPos(const v8::FunctionCallbackInfo<v8::Value>& info);\n"

            "    v8::Local<v8::Object> wrap(void* ptr);\n"

            "} // V8%s\n"

            "} // flappy\n",

             className.c_str(),
             className.c_str());
    return std::string(output.data());

}

std::string generateWrapperCpp(std::string className, std::string methodBodies, std::string methodRefs) {
    std::vector<char> output(10000);
    snprintf(output.data(), output.size(),
            "#include <V8JSManager.h>\n"
            "#include <%s.h>\n"

            "namespace flappy {\n"

            "using namespace v8;\n"

            "namespace V8%s {\n"
            "\n"
            "%s"
            "\n"
            "    Local<Object> wrap(void* ptr) {\n"
            "        auto castedPtr = static_cast<%s*>(ptr);\n"

            "        EscapableHandleScope handle_scope(Isolate::GetCurrent());\n"
            "        Local <Context> context = Local <Context>::New (Isolate::GetCurrent(), Isolate::GetCurrent()->GetCurrentContext());\n"
            "        Context::Scope contextScope (context);\n"

            "        Local<External> jsPtr = External::New(Isolate::GetCurrent(), castedPtr);\n"

            "        Local<FunctionTemplate> funcTemplate = FunctionTemplate::New(Isolate::GetCurrent());\n"
            "        Local<Template> prototype = funcTemplate->PrototypeTemplate();\n"
            "%s"
            "\n"
            "        Local<ObjectTemplate> componentTemplate = funcTemplate->InstanceTemplate();\n"
            "        componentTemplate->SetInternalFieldCount(1);\n"

            "        Local<ObjectTemplate> templ = Local<ObjectTemplate>::New(Isolate::GetCurrent(), componentTemplate);\n"

            "        Local<Object> result = templ->NewInstance(Isolate::GetCurrent()->GetCurrentContext()).ToLocalChecked();\n"

            "        result->SetInternalField(0, jsPtr);\n"

            "        return handle_scope.Escape(result);\n"
            "    }\n"

            "} // V8%s \n"

            "} // flappy \n",
            className.c_str(),
            className.c_str(),
            methodBodies.c_str(),
            className.c_str(),
            methodRefs.c_str(),
            className.c_str());
    return std::string(output.data());
}

std::string generateMethodBody(std::string name, std::string type) {
    std::vector<char> output(10000);
    snprintf(output.data(), output.size(),
            "\n"
            "static void %s(const FunctionCallbackInfo<Value>& info) {\n"
            "    Local<External> field = info.Data().As<External>();\n"
            "    void* ptr = field->Value();\n"
            "    auto objectPtr =  static_cast<%s*>(ptr);\n"
            "}\n"
            "\n",
            name.c_str(),
            type.c_str());
    return std::string(output.data());
}

class MethodPrinter : public MatchFinder::MatchCallback {
public :
    MethodPrinter(std::string path)
        : m_path(path)
    {}

    void writeTextFile(std::string name, std::string data) {
        std::ofstream textFile;
        textFile.open(m_path + "/new_wrappers/" + name);
        textFile << data;
        textFile.close();
    }

    virtual void run(const MatchFinder::MatchResult &Result) {

        if (const CXXRecordDecl *md = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classes")) {
            auto className = md->getNameAsString();

            auto headerFileData = generateWrapperHeader(className);
            auto headerFileName = std::string("V8") + className + ".h";
            writeTextFile(headerFileName, headerFileData);

            m_initializerStream << "wrapperMap[\"flappy::" << className << "]\"] = V8" << className << "::wrap;\n";

            std::cout << "class " << className << std::endl;

            std::stringstream methodBodies;
            std::stringstream methodRefs;

            std::unordered_set<std::string> processedMethods;

            for (auto iter = md->method_begin(); iter != md->method_end(); iter++) {
                if (iter->isInstance() && iter->isUserProvided()) {
                    const auto& methodName = iter->getNameAsString();
                    if (processedMethods.find(methodName) == processedMethods.end()) {
                        processedMethods.insert(methodName);
                        std::cout << "    method " << methodName << std::endl;
                        methodBodies << generateMethodBody(iter->getNameAsString(), className);
                        methodRefs << "        prototype->Set(toV8Str(\"" << methodName;
                        methodRefs << "\"), FunctionTemplate::New(Isolate::GetCurrent()," << methodName;
                        methodRefs << ", jsPtr));\n";
                    }
                }
            }

            auto cppFileData = generateWrapperCpp(className, methodBodies.str(), methodRefs.str());
            auto cppFileName = std::string("V8") + className + ".cpp";
            writeTextFile(cppFileName, cppFileData);
        }
    }

    std::string initializersStr() {
        return m_initializerStream.str();
    }

private:
    std::string m_path;
    std::stringstream m_initializerStream;
};

std::string generateInitializerCpp(std::string initializers) {
    std::vector<char> output(5000);
    snprintf(output.data(), output.size(),

            "#include \"WrapperInitializer.h\"\n"

            "#include <V8JSManager.h>\n"

            "#include \"new_wrappers/V8TransformComponent.h\"\n"

            "namespace flappy {\n"

            "void initV8Wrappers() {\n"
                "%s"
            "}\n"

            "} // flappy\n"
             ,initializers.c_str());
    return std::string(output.data());
}

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
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

    MethodPrinter printer(outputPath.getValue());
    MatchFinder finder;
    finder.addMatcher(methodMatcher, &printer);

    auto result = tool.run(newFrontendActionFactory(&finder).get());

    std::ofstream wrapperInitializerFile;
    wrapperInitializerFile.open(outputPath.getValue() + "/WrapperInitializer.cpp");
    wrapperInitializerFile << generateInitializerCpp(printer.initializersStr());
    wrapperInitializerFile.close();

    return result;
}
