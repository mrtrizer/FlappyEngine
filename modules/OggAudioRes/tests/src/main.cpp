#include <DesktopThread.h>
#include <Entity.h>
#include <OpenALManager.h>
#include <OpenALAudioRes.h>
#include <OpenALListenerComponent.h>
#include <OpenALSourceComponent.h>
#include <OggAudioResFactory.h>
#include <ResManager.h>
#include <StdFileMonitorManager.h>
#include <StdFileLoadManager.h>
#include <ResRepositoryManager.h>
#include <TransformComponent.h>

using namespace flappy;

int main(int, char*[]) {

    auto thread = std::make_shared<DesktopThread>([](SafePtr<Entity> rootEntity) {
            rootEntity->createComponent<OpenALManager>();
            rootEntity->createComponent<OggAudioResFactory>();
            rootEntity->createComponent<StdFileLoadManager>();
            rootEntity->createComponent<ResRepositoryManager>("./resources");
            rootEntity->createComponent<StdFileMonitorManager>();
            auto audioResManager = rootEntity->createComponent<ResManager<OpenALAudioRes>>();
            auto listenerEntity = rootEntity->createEntity();
            listenerEntity->createComponent<OpenALListenerComponent>();
            listenerEntity->createComponent<TransformComponent>();
            auto sourceEntity = rootEntity->createEntity();
            auto source = sourceEntity->createComponent<OpenALSourceComponent>();
            auto audioRes = audioResManager->getRes("boom");
            source->setAudioRes(audioRes);
            source->play();
            source->setGain(0.1f);
            source->setLooping(true);
    });

    auto rootEntity = std::make_shared<Entity>();
    thread->run(rootEntity);

    return 0;
}
