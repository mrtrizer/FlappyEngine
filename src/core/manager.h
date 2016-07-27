#ifndef MANAGER_H
#define MANAGER_H

#include <memory>

template <typename Derived>
class Manager
{
public:
    static Derived* getInst() {
        static Derived inst;
        return &inst;
    }
    Manager() {}

    Manager(const Manager&) = delete;
    void operator=(Manager const&) = delete;
};

#endif // MANAGER_H
