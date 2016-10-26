#include "catch.h"
#include "fakeit.h"

#include <memory>
#include <fstream>

#include <core/managerlist.h>
#include <core/resmanager.h>
#include <core/textfile.h>
#include <qt/TextFileFactoryDesktop.h>

using namespace flappy;
using namespace std;


TEST_CASE( "TextFile::text()") {
    auto resManager = std::make_shared<ResManager>();
    resManager->bind<TextFile>(std::make_shared<TextFileFactoryDesktop>("./"));
    ofstream myfile ("example.txt");
    REQUIRE(myfile.is_open());
    myfile << "Test text";
    myfile.close();
    auto textFile = resManager->get<TextFile>("example.txt");
    REQUIRE(!textFile->ready());
    resManager->update(1);
    REQUIRE(textFile->ready());
    REQUIRE(textFile->resource()->text() == "Test text");
}
