#pragma once

namespace flappy {

template <typename BaseT, typename T>
constexpr bool isBaseOf() {
    return std::is_base_of<BaseT, std::decay_t<T>>::value;
}

}
