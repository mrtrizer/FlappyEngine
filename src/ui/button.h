#pragma once

#include <core/entitymanager.h>

namespace flappy {

class Button: public Component {
public:
    void update(TimeDelta dt);

    void setOnClick(std::function<void()> onClick) {
        m_onClick = onClick;
    }

private:
    bool isInField();
    std::function<void()> m_onClick;
};

} // flappy
