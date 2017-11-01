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
    auto childEntity = rootEntity->createEntity();
    rootEntity->createComponent<V8JSManager>();
    rootEntity->createComponent<TransformComponent>();
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
                "           this.entity.jsComponent('TestComponent1').hello();\n"
                "           let transform = this.entity.component('TransformComponent');\n"
                "           let newPos = {'x':1,'y':2,'z':3};\n"
                "           transform.setPos(newPos);\n"
                "           log(transform.pos().z);\n"
                "           log(this.entity.jsComponent('TestComponent1').name);\n"
                "           this.entity.component('TransformComponent').setAngle2DRad(1.5);\n"
                "           log(\"Angle: \" + this.entity.component('TransformComponent').angle2DRad());\n"
                "       }"
                "   }"
                );
    auto pos = rootEntity->component<TransformComponent>()->pos();
    LOG("x: %f y: %f z: %f", pos.x, pos.y, pos.z);
    rootEntity->events()->post(ComponentBase::UpdateEvent(1.0f));
    pos = rootEntity->component<TransformComponent>()->pos();
    LOG("x: %f y: %f z: %f", pos.x, pos.y, pos.z);
    rootEntity->events()->post(ComponentBase::UpdateEvent(2.0f));
    rootEntity->events()->post(ComponentBase::UpdateEvent(3.0f));
}
