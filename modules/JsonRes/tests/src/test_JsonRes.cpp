#include "catch.h"
#include "fakeit.h"

#include <memory>
#include <fstream>

#include <Heap.hpp>
#include <Hierarchy.hpp>
#include <Entity.hpp>
#include <ResManager.h>
#include <TextRes.h>
#include <DefaultResFactory.h>
#include <JsonRes.h>
#include <StdFileLoadManager.h>
#include <StdFileMonitorManager.h>
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

TEST_CASE( "JsonRes::text()") {
    auto hierarchy = Heap::create<Hierarchy>(Heap::memoryManager());
    auto updateManager = hierarchy->initManager<UpdateManager>();
    hierarchy->initManager<IFileMonitorManager, StdFileMonitorManager>();
    hierarchy->initManager<IFileLoadManager, StdFileLoadManager>();
    auto resRepositoryManager = hierarchy->initManager<ResRepositoryManager>();
    resRepositoryManager->setRepositoryPath("./resources");
    hierarchy->initManager<ResFactory<TextRes>, TextResFactory>();
    auto textResManager = hierarchy->initManager<ResManager<TextRes>>();
    hierarchy->initManager<ResFactory<JsonRes>, DefaultResFactory<JsonRes, JsonRes, TextRes>>();
    auto jsonResManager = hierarchy->initManager<ResManager<JsonRes>>();
    writeToFile("./resources/example.txt", "{\"name\":\"Alfred\"}");
    auto jsonRes = jsonResManager->getRes("example", ExecType::ASYNC);
    REQUIRE(jsonRes->json().empty() == true);
    updateManager->update(1.0f); // two update in case if JsonRes updated before TextRes
    updateManager->update(1.0f); // in such cases dependant resource is reloaded in next update
    jsonRes = std::static_pointer_cast<JsonRes>(jsonResManager->getRes("example", ExecType::ASYNC)->lastRes());
    auto textRes = textResManager->getRes("example", ExecType::ASYNC);
    REQUIRE(jsonRes->json()["name"] == string("Alfred"));
    updateManager->update(1.0f);
    updateManager->update(1.0f);
    writeToFile("./resources/example.txt", "{\"name\":\"Nastya\"}");
    updateManager->update(1.0f);
    updateManager->update(1.0f);
    jsonRes = jsonResManager->getRes("example", ExecType::ASYNC);

    REQUIRE(jsonRes->json()["name"] == string("Nastya"));
}
