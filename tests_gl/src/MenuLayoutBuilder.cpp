#include "MenuLayoutBuilder.h"

#include <SpriteBuilder.h>
#include <ButtonBuilder.h>
#include <TransformComponent.h>

namespace flappy {

void MenuLayoutBuilder::build(std::shared_ptr<Entity> layout) const {
    layout->add(
        SpriteBuilder()
            .spritePath("img_background")
    );

    auto button = layout->add(
        ButtonBuilder()
            .idlePath("start_btn_idle")
            .activatedPath("start_btn_activated")
            .pressedPath("start_btn_pressed")
            .onClick([layout](){
                if (auto managerList = layout->managerList().lock())
                    managerList->events()->post(StartButtonPressed());
            })
    );

    button->transform()->setScale(0.2f);
}

} // flappy
