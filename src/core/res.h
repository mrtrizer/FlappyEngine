#pragma once

#include "classid.h"

namespace flappy {

class IRes {
public:
    virtual ~IRes() = default;
    virtual unsigned id() = 0;
};

template <typename ResT>
class Res: public IRes {
    unsigned id() {return ClassId<IRes, ResT>::id(); }
};

} // flappy
