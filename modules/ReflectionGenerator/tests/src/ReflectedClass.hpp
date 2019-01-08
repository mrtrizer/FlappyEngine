#pragma once

#include <ReflectionMarkers.hpp>

class REFLECT ReflectedClass {
public:
    ReflectedClass() = default;
    int FUNC a(int val) { return val; }
};
