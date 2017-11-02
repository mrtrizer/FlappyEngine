#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/ASTMatchers/ASTMatchers.h"
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

static std::unordered_set<std::string> standardTypes = {
    "int",
    "float",
    "_Bool",
    "std::string",
    "glm::vec2",
    "glm::vec3",
    "glm::quad",
    "std::vector",
    "std::list",
    "std::unordered_map",
    "std::map",
    "std::unordered_set",
    "std::set"
};

static bool isStandard(std::string type) {
    int templateBracketIndex = type.find('<');
    auto baseTypeName = type.substr(0, templateBracketIndex);
    return (standardTypes.find(baseTypeName) != standardTypes.end());
}

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
            "#include <V8BasicTypeWrappers.h>\n"
            "#include <algorithm>\n"
            "#include <%s.h>\n"
            "namespace flappy {\n"
            "using namespace v8;\n"
            "namespace V8%s {\n"
            "\n"
            "%s"
            "\n"
            "Local<Object> wrap(void* ptr) {\n"
            "    auto castedPtr = static_cast<%s*>(ptr);\n"
            "    EscapableHandleScope handle_scope(Isolate::GetCurrent());\n"
            "    Local <Context> context = Local <Context>::New (Isolate::GetCurrent(), Isolate::GetCurrent()->GetCurrentContext());\n"
            "    Context::Scope contextScope (context);\n"
            "    Local<External> jsPtr = External::New(Isolate::GetCurrent(), castedPtr);\n"
            "    Local<FunctionTemplate> funcTemplate = FunctionTemplate::New(Isolate::GetCurrent());\n"
            "    Local<Template> prototype = funcTemplate->PrototypeTemplate();\n"
            "%s"
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
            className.c_str(),
            methodRefs.c_str(),
            className.c_str());
    return std::string(output.data());
}

std::string generateStandardReturn(std::string type) {
    std::vector<char> output(500);
    snprintf(output.data(),
             output.size(),
             "    info.GetReturnValue().Set(toV8<%s>::cast(result));\n",
             type.c_str());
    return std::string(output.data());
}

std::string generateResultWrapper(std::string typeName) {
    if (isStandard(typeName))
        return generateStandardReturn(typeName);
    return "";
}

std::string generateStandardArgWrapper(std::string type, int argIndex) {
    std::vector<char> output(1000);
    snprintf(output.data(), output.size(),
            "    auto arg%d = toCpp<%s>::cast(info[%d]);\n",
             argIndex,
             type.c_str(),
             argIndex);
    return std::string(output.data());
}

std::string generateMethodCall(const CXXMethodDecl* methodDecl, std::string type) {
    std::string name = methodDecl->getNameAsString();

    int generatedParams = 0;
    int requiredParams = methodDecl->param_size();

    std::stringstream argWrappers;
    std::stringstream argRefs;
    {
        int index = 0;
        for (auto paramIter = methodDecl->param_begin(); paramIter != methodDecl->param_end(); paramIter++) {
            std::cout << (*paramIter)->getNameAsString() << " : ";
            auto qualType = (*paramIter)->getType().getNonReferenceType().getAtomicUnqualifiedType();
            auto typeName = qualType.getAsString();
            std::cout << typeName << std::endl;
            if (isStandard(typeName)) {
                argWrappers << generateStandardArgWrapper(typeName, index);
                generatedParams++;
            }
            std::string comma = (index == 0? "" : ",") ;
            argRefs << comma << "arg" << index;
            index++;
        }
    }

    auto resultQualType = methodDecl->getReturnType().getNonReferenceType().getAtomicUnqualifiedType();
    auto resultTypeName = resultQualType.getAsString();
    std::cout << "Return: " << resultTypeName << std::endl;
    std::string resultCodeBlock;
    if (resultTypeName != "void") {
        resultCodeBlock = "auto result = ";
    }

    if (generatedParams == requiredParams) {
        std::vector<char> output(10000);
        snprintf(output.data(), output.size(),
                "%s"
                "\n"
                "    %sobjectPtr->%s(%s);\n"
                "%s\n",
                argWrappers.str().c_str(),
                resultCodeBlock.c_str(),
                name.c_str(),
                argRefs.str().c_str(),
                generateResultWrapper(resultTypeName).c_str());
        return std::string(output.data());
    } else {
        return "    LOGE(\"Wrapper for this method is not implemented\");\n";
    }
}

std::string generateMethodBody(const CXXMethodDecl* methodDecl, std::string type) {
    std::string name = methodDecl->getNameAsString();

    auto methodCallBlock = generateMethodCall(methodDecl, type);

    std::vector<char> output(10000);
    snprintf(output.data(), output.size(),
            "\n"
            "static void method_%s(const FunctionCallbackInfo<Value>& info) {\n"
            "    Local<External> field = info.Data().As<External>();\n"
            "    void* ptr = field->Value();\n"
            "    auto objectPtr =  static_cast<%s*>(ptr);\n"
            "\n"
            "%s"
            "}\n"
            "\n",
            name.c_str(),
            type.c_str(),
            methodCallBlock.c_str());
    return std::string(output.data());
}

class MethodPrinter : public MatchFinder::MatchCallback {
public :
    MethodPrinter(std::string path)
        : m_path(path)
    {}

    void writeTextFile(std::string name, std::string data) {
        std::ofstream textFile;
        textFile.open(m_path + "/wrappers/" + name);
        textFile << data;
        textFile.close();
    }

    virtual void run(const MatchFinder::MatchResult &Result) {
        if (const CXXRecordDecl *md = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classes")) {
            auto className = md->getNameAsString();
            if ((className == "Component") || (className== "Manager") || (className == "JSComponent"))
                return;
            if (m_wrappedClasses.find(className) != m_wrappedClasses.end())
                return;

            m_wrappedClasses.insert(className);

            auto headerFileData = generateWrapperHeader(className);
            auto headerFileName = std::string("V8") + className + ".h";
            writeTextFile(headerFileName, headerFileData);

            m_initializerStream << "wrapperMap[\"flappy::" << className << "]\"] = V8" << className << "::wrap;\n";
            m_initializerHeadersStream << "#include \"wrappers/V8" << className << ".h\"\n";

            std::cout << "class " << className << std::endl;

            std::stringstream methodBodies;
            std::stringstream methodRefs;

            std::unordered_set<std::string> processedMethods;

            for (auto iter = md->method_begin(); iter != md->method_end(); iter++) {
                if (iter->isInstance() && iter->isUserProvided() && iter->getAccess() == AS_public) {
                    const auto& methodName = iter->getNameAsString();
                    if ((methodName != className)
                        && (processedMethods.find(methodName) == processedMethods.end())) {
                        processedMethods.insert(methodName);
                        std::cout << "    method " << methodName << std::endl;
                        methodBodies << generateMethodBody(*iter, className);
                        methodRefs << "        prototype->Set(toV8Str(\"" << methodName;
                        methodRefs << "\"), FunctionTemplate::New(Isolate::GetCurrent(), method_" << methodName;
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

    MethodPrinter printer(outputPath.getValue());
    MatchFinder finder;
    auto methodMatcher = cxxRecordDecl(isClass(), isDefinition(), matchesName(".*(Component|Manager)$")).bind("classes");
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
