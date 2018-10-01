#pragma once

#include <functional>
#include <list>
#include <queue>

namespace flappy {

class UpdateManager {
    friend class UpdateManagerDebugger;
public:
    void update(float dt) {
        for (const auto& updateFunction : m_updateFunctions) {
            if (updateFunction.depth != -1)
                updateFunction.updateFunction(dt);
        }
        std::cout << std::endl;
    }

    int registerUpdateFunction(int depth, const std::function<void(float dt)>& updateFunction) {
        auto destination = std::adjacent_find(m_updateFunctions.begin(), m_updateFunctions.end(),
            [depth](const auto& first, const auto& second) {
                return first.depth <= depth && depth < second.depth;
            });
        m_updateFunctions.insert(destination, UpdateFunction{++m_idCounter, depth, updateFunction});
        return m_idCounter;
    }

    void unregisterUpdateFunction(int id) {
        // FIXME: Could be optimized if remove only first found element
        std::remove_if(m_updateFunctions.begin(), m_updateFunctions.end(), [id](auto& item) { return item.id == id; });
    }

private:
    struct UpdateFunction {
        int id = 0;
        int depth = -1;
        std::function<void(float dt)> updateFunction;
    };

    std::list<UpdateFunction> m_updateFunctions;
    int m_idCounter = 0;
};

}
