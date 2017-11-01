#include <memory>

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

int main(int argc, char *argv[])
{
    initV8Wrappers();

    auto rootEntity = std::make_shared<Entity>();
    rootEntity->createComponent<ResRepositoryManager>("./resources");
    rootEntity->createComponent<StdFileMonitorManager>();
    rootEntity->createComponent<StdFileLoadManager>();
    rootEntity->createComponent<FileResFactory>();
    rootEntity->createComponent<ResManager<TextRes>> ();
    rootEntity->createComponent<V8JSManager>();

    auto childEntity = rootEntity->createEntity();
    childEntity->createComponent<TransformComponent>();
    auto testComponent1Res = rootEntity->manager<ResManager<TextRes>>()->getRes("TestComponent1", ExecType::SYNC);
    childEntity->createComponent<JSComponent>("TestComponent1", testComponent1Res);
    auto testComponent2Res = rootEntity->manager<ResManager<TextRes>>()->getRes("TestComponent2", ExecType::SYNC);
    childEntity->createComponent<JSComponent>("TestComponent2", testComponent2Res);
    auto pos = rootEntity->component<TransformComponent>()->pos();
    LOG("x: %f y: %f z: %f", pos.x, pos.y, pos.z);
    rootEntity->events()->post(ComponentBase::UpdateEvent(1.0f));
    pos = childEntity->component<TransformComponent>()->pos();
    LOG("x: %f y: %f z: %f", pos.x, pos.y, pos.z);
    childEntity->events()->post(ComponentBase::UpdateEvent(2.0f));
    childEntity->events()->post(ComponentBase::UpdateEvent(3.0f));
}
