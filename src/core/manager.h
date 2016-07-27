#ifndef MANAGER_H
#define MANAGER_H

#include <memory>

template <typename Derived>
class Manager
{
public:
    static std::shared_ptr<Derived> getInst() {return inst == nullptr? inst = std::make_shared<Derived>(): inst;}
    Manager() {}
private:
    Manager(const Manager&) = delete;

    static std::shared_ptr<Derived> inst;
};

template <typename Derived>
std::shared_ptr<Derived> Manager<Derived>::inst = nullptr;


#endif // MANAGER_H
