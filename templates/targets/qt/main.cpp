#include <memory>

#include <GlutInit.h>
#include <QtTextureResFactory.h>
#include <ResManager.h>
#include <InputManager.h>
#include <SceneManager.h>
#include <ScreenManager.h>
#include <AppManager.h>
#include <ManagerList.h>
#include <QtFileMonitor.h>
#include <QuadRes.h>
#include <TextureRes.h>
#include <QuadResFactory.h>

#include <{!scene_header!}>

using namespace flappy;
using namespace std;

int main(int argc, char *argv[])
{
    auto managerList = make_shared<ManagerList>();
    managerList->create<AppManager>(argc, argv);
    auto qtFileMonitor = std::make_shared<QtFileMonitor>();
    auto resManager = managerList->create<ResManager>();
    resManager->bindResFactory<TextureRes>(make_shared<QtTextureResFactory>("../../res/", qtFileMonitor));
    resManager->bindResFactory<QuadRes>(make_shared<QuadResFactory>());
    managerList->create<SceneManager>();
    managerList->create<ScreenManager>();
    managerList->create<InputManager>();

    GLUTInit::initGLUT(managerList);

    managerList->init();

    managerList->MGR<SceneManager>()->setScene(make_shared<{!scene_class!}>());

    return GLUTInit::mainLoop();
}
