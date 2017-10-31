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

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace llvm;

auto methodMatcher = cxxRecordDecl(isClass()).bind("classes");

class MethodPrinter : public MatchFinder::MatchCallback {
public :
  virtual void run(const MatchFinder::MatchResult &Result) {

    if (const CXXRecordDecl *md = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classes")) {
      std::cout << "class " << md->getNameAsString() << std::endl;
      for (auto iter = md->method_begin(); iter != md->method_end(); iter++) {
          std::cout << "method " << iter->getNameAsString() << std::endl;
      }
    }
  }
};

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nMore help text...");

int main(int argc, const char **argv) {
  cl::OptionCategory cat("myname", "mydescription");
  CommonOptionsParser optionsParser(argc, argv, cat, 0);

  ClangTool tool(optionsParser.getCompilations(), optionsParser.getSourcePathList());

  MethodPrinter printer;
  MatchFinder finder;
  finder.addMatcher(methodMatcher, &printer);
  return tool.run(newFrontendActionFactory(&finder).get());
}
