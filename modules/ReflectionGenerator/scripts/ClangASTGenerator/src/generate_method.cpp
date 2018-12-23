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

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

std::string generateArgs(const CXXMethodDecl* methodDecl) {
    std::stringstream conditions;

    int index = 0;
    for (auto paramIter = methodDecl->param_begin(); paramIter != methodDecl->param_end(); ++paramIter) {
        auto qualType = (*paramIter)->getType();
        auto typeName = qualType.getAsString();

        replace(typeName, "_Bool", "bool");

        if (paramIter != methodDecl->param_begin())
            conditions << ", ";
        conditions << typeName;
        ++index;
    }

    return conditions.str();
}

std::string generateMethodBody(std::string methodName, std::string className, const std::vector<CXXMethodDecl*> methods) {
    std::stringstream methodConditionBlock;
    for (auto methodDecl : methods) {
        auto resultQualType = methodDecl->getReturnType();//.getNonReferenceType().getAtomicUnqualifiedType();
        auto resultTypeName = resultQualType.getAsString();
        replace(resultTypeName, "_Bool", "bool");
        auto args = generateArgs(methodDecl);

        std::cout << "    method " << methodName << "(" << args << ") -> " << resultTypeName << std::endl;
        //if (methodDecl->isConst()) {
        methodConditionBlock << ".addFunction(\"" << methodName << "\", &" << className << "::" << methodName << ")" << std::endl;
        //}
    }
    return methodConditionBlock.str();
}

std::string generateConstructorBody(std::string className, const std::vector<CXXMethodDecl*> methods, const CXXRecordDecl* classDecl) {
    std::stringstream constructorBody;
    if (!classDecl->isAbstract()) {
        for (auto methodDecl : methods) {
            if (methodDecl->isUserProvided())
                constructorBody << ".addConstructor<" << className << ", " << generateArgs(methodDecl) << ">()" << std::endl;
        }
        // default constructor
        if (classDecl->hasDefaultConstructor())
            constructorBody << ".addConstructor<" << className << ">()" << std::endl;
    }
    return constructorBody.str();
}

GeneratedMethods processMethods(const CXXRecordDecl* classDecl, const std::string& className) {
    std::stringstream methodBodies;

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
        auto overloadedMethods = methodPair.second;
        auto methodName = methodPair.first;
        std::cout << "    method " << methodName << std::endl;
        if (methodName == className) {
            methodBodies << generateConstructorBody(className, overloadedMethods, classDecl);
        } else {
            methodBodies << generateMethodBody(methodName, className, overloadedMethods);
        }
    }

    return GeneratedMethods{methodBodies.str()};
}

