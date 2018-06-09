#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "llvm/Support/CommandLine.h"

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "llvm/ADT/StringRef.h"

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

std::string generateWrapperCpp(std::string className, GeneratedMethods generatedMethods) {
    std::vector<char> output(50000);
    snprintf(output.data(), output.size(),
            "#include <type_traits>\n"
            "#include <rttr/registration>\n"
            "#include <%s.h>\n"
            "\n"
            "namespace flappy {\n"
            "\n"
            "RTTR_REGISTRATION\n"
            "{\n"
            "     using namespace rttr;\n"
            "\n"
            "     registration::class_<%s>(\"%s\")\n"
            "%s\n"
            "( policy::ctor::as_std_shared_ptr );\n"
            "} \n"
            "\n"
            "} // flappy\n"
            "",
            className.c_str(),
            className.c_str(),
            className.c_str(),
            generatedMethods.methodBodies.c_str());
    return std::string(output.data());
}

void writeTextFile(std::string path, std::string data) {
    std::ofstream textFile;
    textFile.open(path);
    textFile << data;
    textFile.close();
}

class ClassHandler : public MatchFinder::MatchCallback {
public :
    ClassHandler(std::string path)
        : m_path(path)
    {}

    virtual void run(const MatchFinder::MatchResult &result) {
        if (const CXXRecordDecl *classDecl = result.Nodes.getNodeAs<clang::CXXRecordDecl>("classes")) {
            auto className = classDecl->getNameAsString();
            if ((className == "Component") || (className== "Manager") || (className == "JSComponent"))
                return;
            if (classDecl->getDescribedClassTemplate() != nullptr) {
                std::cout << "Skip template class " << classDecl->getNameAsString() << std::endl;
                return;
            }
            if (classDecl->getDeclKind() == clang::Decl::ClassTemplatePartialSpecialization) {
                std::cout << "Skip template partial specialization " << classDecl->getNameAsString() << std::endl;
                return;
            }
            if (classDecl->getDeclKind() == clang::Decl::ClassTemplateSpecialization) {
                std::cout << "Skip template specialization " << classDecl->getNameAsString() << std::endl;
                return;
            }
            if (m_wrappedClasses.find(className) != m_wrappedClasses.end())
                return;

            m_wrappedClasses.insert(className);

            std::cout << "class " << className << std::endl;

            auto generatedMethods = processMethods(classDecl, className);

            auto cppFileData = generateWrapperCpp(className, generatedMethods);
            auto cppFileName = std::string("RTTR") + className + ".cpp";
            writeTextFile(m_path + "/" + cppFileName, cppFileData);

            m_sourcesToGeneratedMap.emplace(cppFileName, result.SourceManager->getFilename(classDecl->getLocation()).str());
        }
    }

    std::map<std::string, std::string> sourcesToGeneratedMap() {
        return m_sourcesToGeneratedMap;
    }

private:
    std::string m_path;
    std::unordered_set<std::string> m_wrappedClasses;
    std::map<std::string, std::string> m_sourcesToGeneratedMap;
};

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

    tool.run(newFrontendActionFactory(&finder).get());

    auto map = printer.sourcesToGeneratedMap();

    std::stringstream rttrToCppMap;

    rttrToCppMap << "{" << std::endl;
    for (auto pairIter = map.begin(); pairIter != map.end(); ++pairIter) {
        rttrToCppMap << "\t\"" << outputPath.getValue() << "/" << pairIter->first << "\" : \"" << pairIter->second << "\"";
        if (std::next(pairIter) != map.end())
            rttrToCppMap << ", ";
        rttrToCppMap << std::endl;
    }
    rttrToCppMap << "}" << std::endl;

    std::cout << "RTTR to CPP Map: "<< rttrToCppMap.str();

    writeTextFile(outputPath.getValue() + "/../rttr_to_sources.json", rttrToCppMap.str());

    return 0;
}
