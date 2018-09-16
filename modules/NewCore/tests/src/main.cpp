#include <iostream>

#include <catch.h>
#include <fakeit.h>

#include <Hierarchy.hpp>
#include <Heap.hpp>
#include <Component.hpp>

using namespace fakeit;
using namespace std;
using namespace flappy;

class UpdateManager {
public:
    void update(float dt) {
        for (const auto& updateFunction : m_updateFunctions) {
            updateFunction.second(dt);
        }
    }

    int registerUpdateFunction(const std::function<void(float dt)>& updateFunction) {
        m_updateFunctions.emplace(++idCounter, updateFunction);
        return idCounter;
    }

    void unregisterUpdateFunction(int id) {
        m_updateFunctions.erase(id);
    }

private:
    std::unordered_map<int, std::function<void(float dt)>> m_updateFunctions;
    int idCounter = 0;
};

template <typename DerivedT>
class Updatable {
public:
    Updatable(const Handle<Hierarchy>& hierarchy)
        : m_updateManager(hierarchy->manager<UpdateManager>())
        , m_functionId(m_updateManager->registerUpdateFunction([this](float dt){
                static_cast<DerivedT*>(this)->update(dt);
            }))
    {}
    ~Updatable() {
        if (m_updateManager.isValid())
            m_updateManager->unregisterUpdateFunction(m_functionId);
    }
private:
    Handle<UpdateManager> m_updateManager;
    int m_functionId = 0;
};

class IOtherTestComponent {
public:
    virtual ~IOtherTestComponent() = default;
    virtual void draw() {

    }
};

class ITestManager {
public:
    virtual ~ITestManager() = default;
    virtual void setSomething(int something) = 0;
    virtual int something() = 0;
    virtual float updateTime() const = 0;
    virtual void registerComponent(const Handle<IOtherTestComponent>& component) = 0;
    virtual void unregisterComponent(const Handle<IOtherTestComponent>& component) = 0;
    virtual int componentsRegistered() = 0;
};

class SomeRenderManager {
public:
    SomeRenderManager(const Handle<Hierarchy>&) {}

    void draw() {
        ++m_drawTimes;
    }

    int drawTimes() {
        return m_drawTimes;
    }

private:
    int m_drawTimes;
};

class OtherTestComponent : public IOtherTestComponent, public EnableSelfHandle<OtherTestComponent> {
public:
    OtherTestComponent(const Handle<Entity>& entity)
        : m_entity(entity)
    {
        entity->hierarchy()->manager<ITestManager>()->registerComponent(selfHandle());
    }

    ~OtherTestComponent() {
        m_entity->hierarchy()->manager<ITestManager>()->unregisterComponent(selfHandle());
    }

    void move(float dt) {
        m_position += dt * m_speed;
    }
    void draw() override {
        m_entity->hierarchy()->manager<SomeRenderManager>()->draw();
    }

private:
    static constexpr float m_speed = 2.0f;
    float m_position = 0.0f;
    Handle<Entity> m_entity;
};

class TestManager : public ITestManager, public Updatable<TestManager> {
public:
    using Updatable::Updatable;

    void setSomething(int something) override { m_something = something; }
    int something() override { return m_something; }

    void update(float dt) {
        m_updateTime += dt;
        for (auto component : m_components) {
            component->draw();
        }
    }

    float updateTime() const override { return m_updateTime; }

    void registerComponent(const Handle<IOtherTestComponent>& component) override {
        m_components.push_back(component);
    }

    void unregisterComponent(const Handle<IOtherTestComponent>& component) override {
        auto iter = std::find(m_components.begin(), m_components.end(), component);
        if (iter != m_components.end())
            m_components.erase(iter);
    }

    virtual int componentsRegistered() override {
        return m_components.size();
    }

private:
    int m_something = 0;
    float m_updateTime = 0.0f;
    std::vector<Handle<IOtherTestComponent>> m_components;
};

class TestComponent : public PutAfter<OtherTestComponent>, public Updatable<TestComponent> {
public:
    TestComponent(Handle<Entity> entity)
        : Updatable(entity->hierarchy()), m_entity(entity)
    {}

    void setSomething(int something) {
        m_something = something;
    }

    int something() { return m_something; }

    void update(float dt) {
        m_entity->component<OtherTestComponent>()->move(dt);
        m_updateTime += dt;
    }

    float updateTime() const { return m_updateTime; }

private:
    Handle<Entity> m_entity;
    int m_something = 0;
    float m_updateTime = 0.0f;
};

TEST_CASE( "Hierarchy") {
    auto hierarchy = Heap::create<Hierarchy>();

    hierarchy->initManager<UpdateManager>();

    auto testManager = hierarchy->initManager<ITestManager, TestManager>();
    testManager->setSomething(200);

    hierarchy->initManager<SomeRenderManager>();

    auto entity1 = hierarchy->rootEntity()->createEntity();
    entity1->component<TestComponent>()->setSomething(100);

    REQUIRE(entity1->findComponent<TestComponent>()->something() == 100);
    REQUIRE(hierarchy->manager<ITestManager>()->something() == 200);

    hierarchy->manager<UpdateManager>()->update(1.0f);

    REQUIRE(hierarchy->manager<ITestManager>()->componentsRegistered() == 1);

    auto secondComponent = entity1->createComponent<OtherTestComponent>();

    REQUIRE(hierarchy->manager<ITestManager>()->componentsRegistered() == 2);

    entity1->removeComponent(secondComponent);

    REQUIRE(hierarchy->manager<ITestManager>()->componentsRegistered() == 1);

    REQUIRE(entity1->component<TestComponent>()->updateTime() == 1.0f);
    REQUIRE(hierarchy->manager<ITestManager>()->updateTime() == 1.0f);

    hierarchy->manager<UpdateManager>()->update(1.0f);
    REQUIRE(hierarchy->manager<SomeRenderManager>()->drawTimes() == 2);

    REQUIRE(entity1->component<TestComponent>()->updateTime() == 2.0f);
    REQUIRE(hierarchy->manager<ITestManager>()->updateTime() == 2.0f);
}
