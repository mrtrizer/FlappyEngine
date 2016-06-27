#ifndef CPRESENTER_H
#define CPRESENTER_H

#include <memory>

#include "component.h"
#include "gpresenter.h"

class CPresenter: public ComponentSlot<GPresenter>
{
public:
    using ComponentSlot<GPresenter>::ComponentSlot;
};

#endif // CPRESENTER_H
