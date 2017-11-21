#pragma once

#include <string>
#include <unordered_map>
#include <Tools.h>

namespace flappy {

template <typename ContextT>
class TypeNames {
public:
    static TypeNames& instance() {
        static TypeNames typeNames;
        return typeNames;
    }
    void setName(unsigned index, std::string typeName) {
        if (index >= m_names.size())
            m_names.resize(index + 1);
        m_names[index] = typeName;
        m_indexMap[typeName] = index;
    }
    std::string getName(unsigned index) {
        if (index >= m_names.size())
            return "Unknown";
        else
            return m_names[index];
    }
    unsigned getIndexByName(std::string name) {
        return m_indexMap[name];
    }

    std::vector<std::string> m_names;
    std::unordered_map<std::string, unsigned> m_indexMap;
};

} // flappy
