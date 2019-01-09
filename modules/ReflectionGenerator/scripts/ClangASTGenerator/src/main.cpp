#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "llvm/Support/CommandLine.h"

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "llvm/ADT/StringRef.h"

#include "generate_method.h"
#include "json.hpp"

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
using namespace nlohmann;

std::string createGeneratedReflectionCpp(std::unordered_set<std::string> wrappedClasses) {
    std::stringstream ss;
    ss << "#include \"GeneratedReflection.hpp\"\n";
    ss << "\n";
    ss << "// Unity build: \n";
    for (auto wrappedClass : wrappedClasses)
        ss << "#include \"../Tmp/RR_" << wrappedClass << ".cpp\"\n";
    ss << "\n";
    ss << "flappy::Reflection extractReflectionDb() {\n";
    ss << " flappy::Reflection reflection;\n";
    for (auto wrappedClass : wrappedClasses)
        ss << " register" << wrappedClass << "(reflection);\n";
    ss << "return reflection;\n";
    ss << "}\n";
    ss << "\n";
    return ss.str();
}

std::string generateWrapperCpp(std::string className, GeneratedMethods generatedMethods) {
    std::vector<char> output(50000);
    snprintf(output.data(), output.size(),
            "#include <Type.hpp>\n"
            "#include <BasicTypesReflection.hpp>\n"
            "#include \"AllHeaders.hpp\"\n"
            "\n"
            "namespace flappy {\n"
            "void register%s(Reflection& reflection) {\n"
             "  reflection.registerType<%s>(\"%s\")\n"
             "%s;\n"
            "} \n"
            "} \n"
            "\n"
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

std::string readTextFile(std::string path) {
    std::ifstream textFile;
    textFile.open(path);
    if (!textFile.is_open())
        return {};
    std::stringstream ss;
    ss << textFile.rdbuf();
    textFile.close();
    return ss.str();
}

class ClassHandler : public MatchFinder::MatchCallback {
public :
    ClassHandler(std::string path)
        : m_path(path)
    {}

    virtual void run(const MatchFinder::MatchResult &result) {
        if (const CXXRecordDecl *classDecl = result.Nodes.getNodeAs<clang::CXXRecordDecl>("classes")) {
        
            auto className = classDecl->getNameAsString();
        
            if(!classDecl->hasAttrs())
                return;
            
            clang::AttrVec vec = classDecl->getAttrs();

            if (strcmp(vec[0]->getSpelling(), "annotate") != 0)
                return;
            
            
            SourceLocation startLoc = vec[0]->getRange().getBegin();
            SourceLocation endLoc = vec[0]->getRange().getEnd();
  
            std::cout << "isMacroID: " << startLoc.isMacroID() << std::endl;

            if( startLoc.isMacroID() ) {
                startLoc = result.SourceManager->getImmediateSpellingLoc( startLoc );
                endLoc = result.SourceManager->getImmediateSpellingLoc( endLoc );
            }
            
            auto tokenRange = CharSourceRange::getCharRange(startLoc, endLoc);
            auto annotateValue = Lexer::getSourceText(tokenRange, *result.SourceManager, result.Context->getLangOpts()).str();
            std::cout << className << " : " << vec[0]->getSpelling() << " : " << annotateValue << std::endl;
            if (annotateValue != "annotate(\"reflect\"")
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
            auto cppFileName = std::string("RR_") + className + ".cpp";
            writeTextFile(m_path + "/" + cppFileName, cppFileData);

            m_sourcesToGeneratedMap.emplace(cppFileName, result.SourceManager->getFilename(classDecl->getLocation()).str());
        }
    }

    const std::map<std::string, std::string>& sourcesToGeneratedMap() const {
        return m_sourcesToGeneratedMap;
    }
    
    const std::unordered_set<std::string>& wrappedClasses() const {
        return m_wrappedClasses;
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
    auto methodMatcher = cxxRecordDecl(isClass(), isDefinition()).bind("classes");
    ClassHandler printer(outputPath.getValue() + "/../Tmp");
    finder.addMatcher(methodMatcher, &printer);

    tool.run(newFrontendActionFactory(&finder).get());

    auto map = printer.sourcesToGeneratedMap();

    auto rttrToSourcesPath = outputPath.getValue() + "/../rttr_to_sources.json";

    auto jsonText = readTextFile(rttrToSourcesPath);
    json parsedJson = jsonText.empty() ? json() : json::parse(jsonText);

    std::cout << "RTTR to CPP Map before: "<< parsedJson.dump(4);

    for (auto pairIter = map.begin(); pairIter != map.end(); ++pairIter) {
        auto rttrFilePath = outputPath.getValue() + "/" + pairIter->first;
        auto cppFilePath = pairIter->second;
        parsedJson[rttrFilePath] = cppFilePath;
    }

    auto newJsonText = parsedJson.dump(4);

    std::cout << "RTTR to CPP Map after: "<< newJsonText;

    writeTextFile(rttrToSourcesPath, newJsonText);
    
    writeTextFile(outputPath.getValue() + "/GeneratedReflection.cpp", createGeneratedReflectionCpp(printer.wrappedClasses()));

    return 0;
}
