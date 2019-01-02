#include <Entity.hpp>
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
#include <Heap.hpp>
#include <UpdateManager.hpp>
#include <BasicLoop.hpp>
#include <BasicLoopManager.hpp>

using namespace flappy;

int main(int, char*[]) {

    auto hierarchy = Heap::create<Hierarchy>(Heap::memoryManager());
    auto updateManager = hierarchy->initManager<UpdateManager>();
    auto basicLoopManager = hierarchy->initManager<BasicLoopManager>();
    hierarchy->initManager<OpenALManager>();
    hierarchy->initManager<IFileLoadManager, StdFileLoadManager>();
    hierarchy->initManager<IFileMonitorManager, StdFileMonitorManager>();
    auto repositoryManager = hierarchy->initManager<ResRepositoryManager>();
    repositoryManager->setRepositoryPath("./resources");
    hierarchy->initManager<ResFactory<OpenALAudioRes>, OggAudioResFactory>();
    auto audioResManager = hierarchy->initManager<ResManager<OpenALAudioRes>>();
    auto listenerEntity = hierarchy->rootEntity()->createEntity();
    listenerEntity->createComponent<OpenALListenerComponent>();
    listenerEntity->createComponent<TransformComponent>();
    auto sourceEntity = hierarchy->rootEntity()->createEntity();
    auto source = sourceEntity->createComponent<OpenALSourceComponent>();
    auto audioRes = audioResManager->getRes("boom", ExecType::ASYNC);
    source->setAudioRes(audioRes);
    source->play();
    source->setGain(0.5f);
    source->setLooping(true);

    return BasicLoop(30, basicLoopManager).run([&updateManager](DeltaTime dt) {
        updateManager->update(dt);
    });
}
