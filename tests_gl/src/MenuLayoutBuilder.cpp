#include "MenuLayoutBuilder.h"

#include <SpriteComponent.h>
#include <ButtonBuilder.h>
#include <ButtonComponent.h>
#include <ResManager.h>
#include <TransformComponent.h>
#include <EventRedirectComponent.h>

namespace flappy {

void MenuLayoutComponent::init() {
    // background
    auto sprite = component<TransformComponent>()->createChild();
    sprite->component<SpriteComponent>()->setSpriteResByPath("img_background");

    // start button
    auto button = ButtonBuilder(managerList())
                .idlePath("start_btn_idle")
                .activatedPath("start_btn_activated")
                .pressedPath("start_btn_pressed")
                .build();
    button->component<TransformComponent>()->setScale(0.2f);
    button->component<EventRedirectComponent>()
            ->redirectToManagerList<ButtonComponent::OnButtonClick>(StartButtonPressed());
    component<TransformComponent>()->addChild(button);
}

} // flappy
