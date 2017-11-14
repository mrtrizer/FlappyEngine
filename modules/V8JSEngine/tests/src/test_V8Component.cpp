#include <memory>

#include <catch.h>
#include <Entity.h>
#include <AppManager.h>
#include <TransformComponent.h>
#include <ResManager.h>
#include <SpriteRes.h>
#include <SpriteResFactory.h>
#include <TextureRes.h>
#include <StdFileMonitorManager.h>
#include <ResRepositoryManager.h>
#include <StdFileLoadManager.h>
#include <DefaultResFactory.h>
#include <FileResFactory.h>
#include <DesktopThread.h>
#include <PosixApplication.h>
#include <AtlasResFactory.h>
#include <V8JSManager.h>
#include <JSComponent.h>
#include <WrapperInitializer.h>

using namespace flappy;
using namespace std;

TEST_CASE("JS function call") {
    initV8Wrappers();

    auto rootEntity = std::make_shared<Entity>();
    rootEntity->createComponent<ResRepositoryManager>("./resources");
    rootEntity->createComponent<StdFileMonitorManager>();
    rootEntity->createComponent<StdFileLoadManager>();
    rootEntity->createComponent<FileResFactory>();
    rootEntity->createComponent<ResManager<TextRes>> ();
    rootEntity->createComponent<V8JSManager>();

    auto childEntity = rootEntity->createEntity();
    auto jsRes = rootEntity->manager<ResManager<TextRes>>()->getRes("TrivialComponent", ExecType::SYNC);
    auto jsComponent = childEntity->createComponent<JSComponent>("TrivialComponent", jsRes);
    float result1 = jsComponent->call<float>("multiply", 10, 2);
    REQUIRE(result1 == 20);
    float result2 = jsComponent->call<int>("sumArray", std::vector<int>{10,100,1000});
    REQUIRE(result2 == 1110);
    auto result3 = jsComponent->call<std::string>("sumArray", std::vector<std::string>{"10","100","1000"});
    REQUIRE(result3 == "0101001000");
    auto result4 = jsComponent->call<std::string>("getHelloWorld");
    REQUIRE(result4 == "Hello, world!");
    auto result5 = jsComponent->call<std::vector<std::string>>("splitStr", "str1 str2 str3");
    REQUIRE(result5[1] == "str2");
}
