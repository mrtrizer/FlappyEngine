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

std::shared_ptr<Entity> createRootEntity() {
    wrapperMap = getV8Wrappers();

    auto rootEntity = std::make_shared<Entity>();
    rootEntity->createComponent<ResRepositoryManager>("./resources");
    rootEntity->createComponent<StdFileMonitorManager>();
    rootEntity->createComponent<StdFileLoadManager>();
    rootEntity->createComponent<FileResFactory>();
    rootEntity->createComponent<ResManager<TextRes>> ();
    rootEntity->createComponent<V8JSManager>();

    return rootEntity;
}

TEST_CASE("Call of a JS function") {
    auto rootEntity = createRootEntity();

    auto childEntity = rootEntity->createEntity();
    auto jsRes = rootEntity->manager<ResManager<TextRes>>()->getRes("TrivialComponent", ExecType::SYNC);
    auto jsComponent = childEntity->createComponent<JSComponent>("TrivialComponent", jsRes);

    {
        auto result1 = jsComponent->call("multiply", 10, 2);
        REQUIRE(result1.as<float>() == 20);
    }

    {
        auto result2 = jsComponent->call("sumArray", std::vector<int>{10,100,1000});
        REQUIRE(result2.as<int>() == 1110);
    }

    {
        auto result3 = jsComponent->call("sumArray", std::vector<std::string>{"10","100","1000"});
        REQUIRE(result3.as<std::string>() == "0101001000");
    }

    {
        auto result4 = jsComponent->call("getHelloWorld");
        REQUIRE(result4.as<std::string>() == "Hello, world!");
    }

    {
        auto result5 = jsComponent->call("splitStr", "str1 str2 str3");
        REQUIRE(result5.as<std::vector<std::string>>()[1] == "str2");
    }

    {
        jsComponent->call("printStr", "Hello, world!");
    }

    {
        auto transform = std::make_shared<TransformComponent>();
        transform->setPos({10, 20, 30});
        auto result = jsComponent->call("extractZPosFromObject", transform);
        REQUIRE(result.as<float>() == 30.0f);
    }

    {
        auto result = jsComponent->call("extractZPosFromSavedObject");
        REQUIRE(result.as<float>() == 30.0f);
    }

    {
        auto result = jsComponent->call("getSavedObject");
        REQUIRE(result.as<std::shared_ptr<TransformComponent>>()->pos().x == 10.0f);
    }

    {
        auto result = jsComponent->call("getSavedObject");
        REQUIRE(result.as<SafePtr<TransformComponent>>()->pos().x == 10.0f);
    }

    // Pass a dictionary to js and return back
}

TEST_CASE("Access to Cpp components from a JS component") {
    auto rootEntity = createRootEntity();

    auto childEntity = rootEntity->createEntity();
    auto jsRes = rootEntity->manager<ResManager<TextRes>>()->getRes("CppAccessComponent", ExecType::SYNC);
    auto transformComponent = childEntity->createComponent<TransformComponent>();
    auto jsComponent = childEntity->createComponent<JSComponent>("CppAccessComponent", jsRes);

    {
        jsComponent->call("setAngleToTransform", 1.0f);
        REQUIRE(transformComponent->angle2DRad() == 1.0f);
        auto newAngle = jsComponent->call("getAngleFromTransform");
        REQUIRE(newAngle.as<float>() == 1.0f);
    }

    {
        auto targetPos = glm::vec3(1.0f, 2.0f, 3.0f);
        jsComponent->call("setPosToTransform", targetPos);
        REQUIRE(transformComponent->pos() == targetPos);
        auto newPos = jsComponent->call("getPosFromTransform");
        auto vec3NewPos = newPos.as<glm::vec3>();
        REQUIRE(vec3NewPos == targetPos);
    }
}

TEST_CASE("Access to another JS component from a JS component") {
    auto rootEntity = createRootEntity();

    auto childEntity = rootEntity->createEntity();
    auto jsRes = rootEntity->manager<ResManager<TextRes>>()->getRes("JsAccessComponent", ExecType::SYNC);
    auto jsComponent = childEntity->createComponent<JSComponent>("JsAccessComponent", jsRes);
    auto testJsRes = rootEntity->manager<ResManager<TextRes>>()->getRes("TestComponent1", ExecType::SYNC);
    auto testJsComponent = childEntity->createComponent<JSComponent>("TestComponent1", testJsRes);

    {
        auto result = jsComponent->call("callCalcSum", 1.0f, 2.0f);
        REQUIRE(result.as<float>() == 3.0f);
    }
}

TEST_CASE("JS component initialization") {
    auto rootEntity = createRootEntity();

    auto childEntity = rootEntity->createEntity();
    auto jsRes = rootEntity->manager<ResManager<TextRes>>()->getRes("InitTestComponent", ExecType::SYNC);
    auto jsComponent = childEntity->createComponent<JSComponent>("InitTestComponent", jsRes);
    REQUIRE(jsComponent->field("constructorCounter").as<int>() == 1);
    REQUIRE(jsComponent->field("initCounter").as<int>() == 1);
    REQUIRE(jsComponent->field("deinitCounter").as<int>() == 0);
    REQUIRE(jsComponent->field("updateCounter").as<int>() == 0);
    rootEntity->events()->post(ComponentBase::UpdateEvent(1.0f));
    REQUIRE(jsComponent->field("updateCounter").as<int>() == 1);
}

TEST_CASE("Passing and receiving events to JS component") {

}
