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

using namespace flappy;
using namespace std;

int main(int argc, char *argv[])
{
    auto rootEntity = std::make_shared<Entity>();
    rootEntity->createComponent<V8JSManager>();
    rootEntity->createComponent<JSComponent>(
                "TestComponent",
                "class TestComponent extends Component {"
                "   constructor() {"
                "       super();"
                "       log('Constructor');"
                "   }"
                "   init() {"
                "       log('Init');"
                "   }"
                "   deinit() {"
                "       log('Deinit');"
                "   }"
                "   update(dt) {"
                "       log(dt.toString());"
                "   }"
                "}"
                ""
                "function constructJsObject() {"
                "   log('constructJsObject start');"
                "   let testComponent = new TestComponent();"
                "   log('constructJsObject end');"
                "   return testComponent;"
                "}");
    rootEntity->events()->post(ComponentBase::UpdateEvent(1.0f));
    rootEntity->events()->post(ComponentBase::UpdateEvent(2.0f));
    rootEntity->events()->post(ComponentBase::UpdateEvent(3.0f));
}
