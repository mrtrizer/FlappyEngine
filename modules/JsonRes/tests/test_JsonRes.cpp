#include "catch.h"
#include "fakeit.h"

#include <memory>
#include <fstream>

#include <ResManager.h>
#include <TextRes.h>
#include <DefaultResFactory.h>
#include <JsonRes.h>
#include <StdTextFileResFactory.h>
#include <QtFileMonitor.h>
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
    resManager->update(1); // two update in case if JsonRes updated before TextRes
    resManager->update(1); // in such cases dependant resource is reloaded in next update
    jsonRes = resManager->getRes<JsonRes>("example.txt");
    REQUIRE(jsonRes->json()["name"] == string("Alfred"));
    resManager->update(1);
    resManager->update(1);
    writeToFile("example.txt", "{\"name\":\"Nastya\"}");
    resManager->update(1);
    resManager->update(1);
    jsonRes = resManager->getRes<JsonRes>("example.txt");
    REQUIRE(jsonRes->json()["name"] == string("Nastya"));
}
