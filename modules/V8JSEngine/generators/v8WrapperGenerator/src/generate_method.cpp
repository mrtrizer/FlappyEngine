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

std::string generateResultWrapper(std::string typeName) {
    if (isStandard(typeName)) {
        std::vector<char> output(500);
        snprintf(output.data(),
                 output.size(),
                 "        info.GetReturnValue().Set(toV8<%s>::cast(result));\n",
                 typeName.c_str());
        return std::string(output.data());
    } else {
        return "";
    }
}

std::string generateStandardArgWrapper(std::string type, int argIndex) {
    std::vector<char> output(1000);
    snprintf(output.data(), output.size(),
             "        auto arg%d = toCpp<%s>::cast(info[%d]);\n",
             argIndex,
             type.c_str(),
             argIndex);
    return std::string(output.data());
}

struct GeneratedArgs {
    bool success;
    std::string argWrappers;
    std::string argRefs;
};

GeneratedArgs generateArgs(const CXXMethodDecl* methodDecl) {
    std::string name = methodDecl->getNameAsString();

    int generatedParams = 0;
    int requiredParams = methodDecl->param_size();

    std::stringstream argWrappers;
    std::stringstream argRefs;
    {
        int index = 0;
        for (auto paramIter = methodDecl->param_begin(); paramIter != methodDecl->param_end(); paramIter++) {
            std::cout << "         " << (*paramIter)->getNameAsString() << " : ";
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

    bool success = (requiredParams == generatedParams);
    GeneratedArgs generatedArgs = {success, argWrappers.str(), argRefs.str()};
    return generatedArgs;
}

std::string generateCallConditions(const CXXMethodDecl* methodDecl) {
    int requiredParamQty = methodDecl->param_size();
    std::stringstream conditions;

    conditions << "(info.Length() == " << requiredParamQty << ")";
    int index = 0;
    for (auto paramIter = methodDecl->param_begin(); paramIter != methodDecl->param_end(); ++paramIter) {
        auto qualType = (*paramIter)->getType().getNonReferenceType().getAtomicUnqualifiedType();
        auto typeName = qualType.getAsString();
        conditions << " && " << "isMatchCpp<" << typeName << ">::check(info[" << index << "])";
        ++index;
    }

    return conditions.str();
}

std::string generateMethodCall(const CXXMethodDecl* methodDecl, std::string type) {
    std::string name = methodDecl->getNameAsString();

    auto generatedArgs = generateArgs(methodDecl);

    auto resultQualType = methodDecl->getReturnType().getNonReferenceType().getAtomicUnqualifiedType();
    auto resultTypeName = resultQualType.getAsString();
    std::cout << "         Return: " << resultTypeName << std::endl;
    std::string resultCodeBlock;
    if (resultTypeName != "void") {
        resultCodeBlock = "auto result = ";
    }

    if (generatedArgs.success) {
        std::vector<char> output(10000);
        snprintf(output.data(), output.size(),
                "    if (%s) {\n"
                "%s\n"
                "        %sobjectPtr->safePtr()->%s(%s);\n"
                "%s\n"
                "        return;\n"
                "    }\n",
                generateCallConditions(methodDecl).c_str(),
                generatedArgs.argWrappers.c_str(),
                resultCodeBlock.c_str(),
                name.c_str(),
                generatedArgs.argRefs.c_str(),
                generateResultWrapper(resultTypeName).c_str());
        return std::string(output.data());
    } else {
        return "    LOGE(\"Wrapper for this method is not implemented\");\n";
    }
}

std::string generateMethodBody(std::string methodName, std::string className, const std::vector<CXXMethodDecl*> methods) {
    std::stringstream methodConditionBlock;
    for (auto methodDecl : methods) {
        methodConditionBlock << generateMethodCall(methodDecl, className);
    }

    std::vector<char> output(10000);
    snprintf(output.data(), output.size(),
            "\n"
            "static void method_%s(const FunctionCallbackInfo<Value>& info) {\n"
            "    Local<External> field = info.This()->GetHiddenValue(toV8Str(\"cpp_ptr\")).As<External>();\n"
            "    void* ptr = field->Value();\n"
            "    auto objectPtr =  static_cast<CppObjectHolder<%s>*>(ptr);\n"
            "\n"
            "%s"
            "}\n"
            "\n",
            methodName.c_str(),
            className.c_str(),
            methodConditionBlock.str().c_str());
    return std::string(output.data());
}

std::string generateConstructorCall(std::string argWrappers, std::string className, std::string argRefs) {
    std::vector<char> output(10000);
    snprintf(output.data(), output.size(),
            "%s\n"
            "    auto sharedPtr = std::make_shared<%s>(%s);\n"
            "    auto ptr = new SharedPtrHolder<%s>(sharedPtr);\n"
            "    Local<External> jsPtr = External::New(Isolate::GetCurrent(), ptr);\n"
            "    setMethods(info.This(), jsPtr);\n"
            "    UniquePersistent<External> external(Isolate::GetCurrent(), jsPtr);\n"
            "    external.SetWeak<CppObjectHolderBase>(ptr, v8DestroyHolder, WeakCallbackType::kParameter);\n"
            "    persistentHolder.push_back(std::move(external));\n"
            "    info.This()->SetHiddenValue(toV8Str(\"cpp_ptr\"), jsPtr);\n"
            "\n",
             argWrappers.c_str(),
             className.c_str(),
             argRefs.c_str(),
             className.c_str());
    return std::string(output.data());
}

std::string generateConditionalConstructorCall(const CXXMethodDecl* methodDecl, std::string className) {
    auto args = generateArgs(methodDecl);
    if (args.success) {
        std::vector<char> output(10000);
        snprintf(output.data(), output.size(),
                "if (%s) {\n"
                "%s\n"
                "}\n"
                "\n",
                 generateCallConditions(methodDecl).c_str(),
                 generateConstructorCall(args.argWrappers, className, args.argRefs).c_str());
        return std::string(output.data());
    } else {
        return "";
    }
}

std::string generateConstructorBody(const std::vector<CXXMethodDecl*> methods, const CXXRecordDecl* classDecl) {
    auto className = classDecl->getNameAsString();
    std::stringstream constructorBody;
    if (!classDecl->isAbstract()) {
        for (auto methodDecl : methods) {
            constructorBody << generateConditionalConstructorCall(methodDecl, className);
        }
        // default constructor
        if (classDecl->hasDefaultConstructor())
            constructorBody << generateConstructorCall("", className, "");
    }
    std::vector<char> output(10000);
    snprintf(output.data(), output.size(),
            "\n"
            "static void method_constructor(const FunctionCallbackInfo<Value>& info) {\n"
            "%s\n"
            "}\n"
            "\n",
             constructorBody.str().c_str());
    return std::string(output.data());
}

GeneratedMethods processMethods(const CXXRecordDecl* classDecl, const std::string& className) {
    std::stringstream methodBodies;
    std::stringstream methodRefs;

    std::unordered_map<std::string, std::vector<CXXMethodDecl*>> methods;

    for (auto iter = classDecl->method_begin(); iter != classDecl->method_end(); iter++) {
        const auto& methodName = iter->getNameAsString();
        if ((iter->isInstance() && iter->isUserProvided() && iter->getAccess() == AS_public && (methodName != "~" + className))
                || (methodName == className)) {
            methods[methodName].push_back(*iter);
        }
    }

    std::unordered_set<std::string> processedMethods;
    for (auto methodPair : methods) {
        auto methods = methodPair.second;
        auto methodName = methodPair.first;
        std::cout << "    method " << methodName << std::endl;
        if (methodName == className) {
            methodBodies << generateConstructorBody(methods, classDecl);
        } else {
            methodBodies << generateMethodBody(methodName, className, methods);
            methodRefs << "        prototype->Set(toV8Str(\"" << methodName;
            methodRefs << "\"), Function::New(Isolate::GetCurrent(), method_" << methodName;
            methodRefs << ", jsPtr));\n";
        }
    }

    return GeneratedMethods{methodBodies.str(), methodRefs.str()};
}

