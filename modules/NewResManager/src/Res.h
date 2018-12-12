#pragma once

#include <list>
#include <memory>

#include "ResBase.h"

namespace flappy {

template <typename DerivedT>
class Res: public ResBase {
public:

    std::shared_ptr<DerivedT> nextRes() {
        return std::static_pointer_cast<DerivedT>(nextResBase());
    }

    std::shared_ptr<DerivedT> lastRes() {
        return std::static_pointer_cast<DerivedT>(lastResBase());
    }

};

} // flappy
