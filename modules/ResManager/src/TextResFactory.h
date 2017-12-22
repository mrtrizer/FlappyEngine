#pragma once

#include <string>

#include "ResFactory.h"
#include "IFileLoadManager.h"
#include "IFileMonitorManager.h"
#include "ResRepositoryManager.h"
#include "TextRes.h"

namespace flappy {

class IFileMonitorManager;
class Entity;

/// @addtogroup ResManager
/// @{

class TextResFactory : public ResFactory<TextRes>
{
public:
    TextResFactory();

    std::shared_ptr<ResBase> load(const std::string& name, ExecType execType) final;
    std::shared_ptr<ResBase> create(const std::string& name) final;
    bool changed(const std::string& name) final;
};

/// @}

} // flappy
