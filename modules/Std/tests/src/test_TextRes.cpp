#include "catch.h"
#include "fakeit.h"

#include <memory>
#include <fstream>

#include <ResManager.h>
#include <TextRes.h>
#include <Entity.hpp>
#include <Heap.hpp>
#include <Hierarchy.hpp>
#include <StdFileMonitorManager.h>
#include <StdFileLoadManager.h>
#include <TextResFactory.h>
#include <thread>

using namespace flappy;
using namespace std;

namespace {

void writeToFile(std::string name, std::string text) {
    ofstream myfile (name);
    myfile << text;
    myfile.close();
    std::this_thread::sleep_for(1s); // sleep for correct resource update checking
}

}

TEST_CASE( "TextRes::text()") {
    auto hierarchy = Heap::create<Hierarchy>(Heap::memoryManager());
    auto updateManager = hierarchy->initManager<UpdateManager>();
    hierarchy->initManager<IFileMonitorManager, StdFileMonitorManager>();
    hierarchy->initManager<IFileLoadManager, StdFileLoadManager>();
    auto resRepositoryManager = hierarchy->initManager<ResRepositoryManager>();
    resRepositoryManager->setRepositoryPath("./resources");
    hierarchy->initManager<ResFactory<TextRes>, TextResFactory>();
    auto textResManager = hierarchy->initManager<ResManager<TextRes>>();

    updateManager->update(1.0f);
    writeToFile("./resources/example.txt", "Test text");
    auto textFile = textResManager->getRes("example", ExecType::ASYNC);
    REQUIRE(textFile->text() == "");
    updateManager->update(1.0f);
    textFile = textResManager->getRes("example", ExecType::ASYNC);
    REQUIRE(textFile->text() == "Test text");
    updateManager->update(1.0f);
    REQUIRE(textFile->nextRes() == textFile); // res should not be updated during update
    writeToFile("./resources/example.txt", "Test text 2");
    updateManager->update(1.0f);
    textFile = textResManager->getRes("example", ExecType::ASYNC);
    REQUIRE(textFile->text() == "Test text 2");
}
