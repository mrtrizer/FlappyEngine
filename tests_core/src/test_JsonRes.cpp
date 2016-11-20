#include "catch.h"
#include "fakeit.h"

#include <memory>
#include <fstream>

#include <core/managerlist.h>
#include <managers/ResManager/ResManager.h>
#include <resources/TextRes.h>
#include <managers/ResManager/DefaultResFactory.h>
#include <resources/JsonRes.h>
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

TEST_CASE( "JsonRes::text()") {
    auto resManager = make_shared<ResManager>();
    auto qtFileMonitor = make_shared<QtFileMonitor>();
    resManager->bindResFactory<JsonRes>(std::make_shared<DefaultResFactory<JsonRes, TextRes>>());
    resManager->bindResFactory<TextRes>(std::make_shared<StdTextFileResFactory>("./", qtFileMonitor));
    writeToFile("example.txt", "{\"name\":\"Alfred\"}");
    auto jsonRes = resManager->getRes<JsonRes>("example.txt");
    REQUIRE(jsonRes->json().empty() == true);
    resManager->update(1);
    jsonRes = resManager->getRes<JsonRes>("example.txt");
    REQUIRE(jsonRes->json()["name"] == string("Alfred"));
    resManager->update(1);
    writeToFile("example.txt", "{\"name\":\"Nastya\"}");
    resManager->update(1);
    jsonRes = resManager->getRes<JsonRes>("example.txt");
    REQUIRE(jsonRes->json()["name"] == string("Nastya"));
}
