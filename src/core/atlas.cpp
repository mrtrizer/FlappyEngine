#include "atlas.h"

namespace flappy {

Atlas::Atlas()
{

}

Tools::Rect Atlas::getRect(std::string) {
    return Tools::Rect{0.0f, 0.0f, 1.0f, 1.0f};
}

} // flappy

