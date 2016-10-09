#pragma once

#include <functional>

#include "tools.h"

namespace flappy {

template <typename Context, typename ... Dependencies>
class Dependent {
    friend class ManagerList;
protected:
    using CheckFunction = std::function<bool(unsigned)>;

    static bool dependenceSatisfied(CheckFunction check) {
        return defined<sizeof...(Dependencies), Dependencies...>(check);
    }

    template <int n = 0> inline
    static bool defined(CheckFunction) {
        return true;
    }

    template <int n, typename Dependency, typename ... Args>
    static bool defined(CheckFunction check) {
        if (check(ClassId<Context, Dependency>::id()))
            return defined<sizeof...(Args), Args...>(check);
        else
            return ERROR_MSG(false, "Dependent Manager is not defined.");
    }
};

} // flappy
