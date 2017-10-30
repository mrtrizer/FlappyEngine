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
                "\n"
                "function constructJsObject(wrapper) {\n"
                "   let ComponentWrapper = function () {"
                "       log(this.initialized.toString());\n"
                "   }\n"
                "\n"
                "   ComponentWrapper.prototype = wrapper;\n"
                "\n"
                "   class Component extends ComponentWrapper {\n"
                "       constructor() {\n"
                "           super();\n"
                "           log('Component constructor');"
                "           log(this.initialized.toString());\n"
                "       }\n"
                "   }\n"
                "   class TestComponent extends Component {\n"
                "       constructor() {"
                "           super();"
                "           log('Constructor');"
                "           log(this.initialized.toString());"
                "       }"
                "       init() {"
                "           log('Init');"
                "       }"
                "       deinit() {"
                "           log('Deinit');"
                "       }"
                "       update(dt) {"
                "           log(dt.toString());"
                "           log(this.initialized.toString());"
                "      }"
                "   }"
                "   log('constructJsObject start');"
                "   let testComponent = new TestComponent();"
                "   log('constructJsObject end');"
                "   testComponent.update(100);"
                "   return testComponent;"
                "}");
    rootEntity->events()->post(ComponentBase::UpdateEvent(1.0f));
    rootEntity->events()->post(ComponentBase::UpdateEvent(2.0f));
    rootEntity->events()->post(ComponentBase::UpdateEvent(3.0f));
}
