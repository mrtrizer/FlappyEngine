#pragma once

#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "llvm/Support/CommandLine.h"

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

struct GeneratedMethods {
    std::string methodBodies;
};

GeneratedMethods processMethods(const clang::CXXRecordDecl* classDecl, const std::string &className);
