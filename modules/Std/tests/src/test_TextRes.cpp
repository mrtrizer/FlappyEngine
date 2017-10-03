#include "catch.h"
#include "fakeit.h"

#include <memory>
#include <fstream>

#include <ResManager.h>
#include <TextRes.h>
#include <Entity.h>
#include <StdFileMonitorManager.h>
#include <StdFileLoadManager.h>
#include <FileResFactory.h>
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
    auto rootEntity = std::make_shared<Entity>();
    rootEntity->createComponent<StdFileMonitorManager>();
    rootEntity->createComponent<StdFileLoadManager>();
    auto resRepositoryManager = rootEntity->createComponent<ResRepositoryManager>("./resources");
    rootEntity->createComponent<FileResFactory>();
    auto textResManager = rootEntity->createComponent<ResManager<TextRes>>();

    rootEntity->events()->post(ComponentBase::UpdateEvent(1.0f));
    writeToFile("./resources/example.txt", "Test text");
    auto textFile = textResManager->getRes("example");
    REQUIRE(textFile->text() == "");
    rootEntity->events()->post(ComponentBase::UpdateEvent(1.0f));
    textFile = textResManager->getRes("example");
    REQUIRE(textFile->text() == "Test text");
    rootEntity->events()->post(ComponentBase::UpdateEvent(1.0f));
    REQUIRE(textFile->nextRes() == textFile); // res should not be updated during update
    writeToFile("./resources/example.txt", "Test text 2");
    rootEntity->events()->post(ComponentBase::UpdateEvent(1.0f));
    textFile = textResManager->getRes("example");
    REQUIRE(textFile->text() == "Test text 2");
}
