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

using namespace flappy;
using namespace std;

int main(int argc, char *argv[])
{
    PosixApplication application;
    auto currentThread = std::make_shared<DesktopThread>(
        [argc, argv](SafePtr<Entity> rootEntity) {
            auto v8JsManager = rootEntity->createComponent<V8JSManager>();
        });
    return application.runThread(currentThread);
}
