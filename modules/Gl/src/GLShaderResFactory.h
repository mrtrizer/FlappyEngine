#pragma once

#include <memory>
#include <vector>

#include <IResFactory.h>

namespace flappy {

class GLShaderResFactory: public IResFactory {
public:
    std::shared_ptr<Res> load(const ResInfo& resInfo, SafePtr<Entity>) override;
    std::shared_ptr<Res> create(const std::string& name, SafePtr<Entity>) override;
};

} // flappy
