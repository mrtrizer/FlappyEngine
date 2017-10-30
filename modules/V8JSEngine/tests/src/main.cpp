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
    auto childEntity = rootEntity->createEntity();
    rootEntity->createComponent<V8JSManager>();
    rootEntity->createComponent<JSComponent>(
                "TestComponent1",
                "   class TestComponent1 extends Component {\n"
                "       constructor() {"
                "           super();"
                "           log('Constructor 1');"
                "           log(this.initialized.toString());"
                "           this.name = 'Vasia';"
                "       }"
                "       init() {"
                "           log('Init');"
                "       }"
                "       deinit() {"
                "           log('Deinit');"
                "       }"
                "       hello() {"
                "           log('Hello, Im ' + this.name);"
                "       }"
                "       update(dt) {"
                "       }"
                "   }"
                );
    rootEntity->createComponent<JSComponent>(
                "TestComponent2",
                "   class TestComponent2 extends Component {\n"
                "       constructor() {"
                "           super();"
                "           log('Constructor 2');"
                "           log(this.initialized.toString());"
                "       }"
                "       init() {"
                "           log('Init');"
                "       }"
                "       deinit() {"
                "           log('Deinit');"
                "       }"
                "       update(dt) {"
                "           this.entity.component('TestComponent1').hello();"
                "           log(this.entity.component('TestComponent1').name);\n"
                "      }"
                "   }"
                );
    rootEntity->events()->post(ComponentBase::UpdateEvent(1.0f));
    rootEntity->events()->post(ComponentBase::UpdateEvent(2.0f));
    rootEntity->events()->post(ComponentBase::UpdateEvent(3.0f));
}
