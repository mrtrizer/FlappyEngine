#include "MenuLayoutBuilder.h"

#include <SpriteBuilder.h>
#include <ButtonBuilder.h>
#include <TransformComponent.h>

namespace flappy {

std::shared_ptr<Entity> MenuLayoutBuilder::build() const {

    auto layout = std::make_shared<Entity>();

    layout->transform()->addChild(SpriteBuilder(managerList())
            .spritePath("img_background")
            .build()
    );

    auto button = ButtonBuilder(managerList())
            .idlePath("start_btn_idle")
            .activatedPath("start_btn_activated")
            .pressedPath("start_btn_pressed")
            .onClick([layout](){
                if (auto managerList = layout->managerList().lock())
                    managerList->events()->post(StartButtonPressed());
            })
            .build();
    button->transform()->setScale(0.2f);
    layout->transform()->addChild(button);

    return layout;
}

} // flappy
