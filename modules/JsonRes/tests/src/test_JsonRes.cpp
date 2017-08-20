#include "catch.h"
#include "fakeit.h"

#include <memory>
#include <fstream>

#include <ResManager.h>
#include <TextRes.h>
#include <DefaultResFactory.h>
#include <JsonRes.h>
#include <StdFileLoadManager.h>
#include <StdFileMonitorManager.h>
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

TEST_CASE( "JsonRes::text()") {
    auto rootEntity = std::make_shared<Entity>();
    rootEntity->createComponent<StdFileMonitorManager>();
    rootEntity->createComponent<StdFileLoadManager>();
    auto resRepositoryManager = rootEntity->createComponent<ResRepositoryManager>("./resources");
    auto textResManager = rootEntity->createComponent<ResManager<TextRes>>();
    textResManager->bindResFactory(std::make_shared<FileResFactory>());
    auto jsonResManager = rootEntity->createComponent<ResManager<JsonRes>>();
    jsonResManager->bindResFactory(std::make_shared<DefaultResFactory<JsonRes, TextRes>>());
    writeToFile("./resources/example.txt", "{\"name\":\"Alfred\"}");
    auto jsonRes = jsonResManager->getRes("example");
    REQUIRE(jsonRes->json().empty() == true);
    rootEntity->events()->post(ComponentBase::UpdateEvent(1)); // two update in case if JsonRes updated before TextRes
    rootEntity->events()->post(ComponentBase::UpdateEvent(1)); // in such cases dependant resource is reloaded in next update
    jsonRes = std::static_pointer_cast<JsonRes>(jsonResManager->getRes("example")->lastRes());
    auto textRes = textResManager->getRes("example");
    REQUIRE(jsonRes->json()["name"] == string("Alfred"));
    rootEntity->events()->post(ComponentBase::UpdateEvent(1));
    rootEntity->events()->post(ComponentBase::UpdateEvent(1));
    writeToFile("./resources/example.txt", "{\"name\":\"Nastya\"}");
    rootEntity->events()->post(ComponentBase::UpdateEvent(1));
    rootEntity->events()->post(ComponentBase::UpdateEvent(1));
    jsonRes = jsonResManager->getRes("example");
    REQUIRE(jsonRes->json()["name"] == string("Nastya"));
}
