#pragma once

#include "classid.h"

namespace flappy {

class IRes {
public:
    IRes() = default;
    IRes(const IRes&) = default;
    IRes(IRes&&) = default;
    virtual ~IRes() = default;
    IRes& operator=(const IRes&) = default;
    IRes& operator=(IRes&&) = default;

    virtual unsigned id() = 0;
};

template <typename ResT>
class Res: public IRes {
    unsigned id() override final { return ClassId<IRes, ResT>::id(); }
};

} // flappy
