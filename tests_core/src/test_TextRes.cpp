#include "catch.h"
#include "fakeit.h"

#include <memory>
#include <fstream>

#include <core/managerlist.h>
#include <managers/ResManager/ResManager.h>
#include <resources/TextRes.h>
#include <qt/resources/StdTextFileResFactory.h>
#include <qt/QtFileMonitor.h>
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
    auto resManager = std::make_shared<ResManager>();
    auto qtFileMonitor = std::make_shared<QtFileMonitor>();
    resManager->bindResFactory<TextRes>(std::make_shared<StdTextFileResFactory>("./", qtFileMonitor));
    writeToFile("example.txt", "Test text");
    auto textFile = resManager->getRes<TextRes>("example.txt");
    REQUIRE(textFile->text() == "");
    resManager->update(1);
    textFile = resManager->getRes<TextRes>("example.txt");
    REQUIRE(textFile->text() == "Test text");
    resManager->update(1);
    REQUIRE(textFile->nextRes() == textFile); // res should not be updated during update
    writeToFile("example.txt", "Test text 2");
    resManager->update(1);
    textFile = resManager->getRes<TextRes>("example.txt");
    REQUIRE(textFile->text() == "Test text 2");
}
