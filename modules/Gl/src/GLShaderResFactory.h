#pragma once

#include <memory>
#include <vector>

#include <ResFactory.h>
#include <ShaderRes.h>

namespace flappy {

class GLShaderResFactory: public ResFactory<ShaderRes> {
public:
    GLShaderResFactory();

    std::shared_ptr<ResBase> load(const std::string& name, ExecType execType) final;
    std::shared_ptr<ResBase> create(const std::string& name) final;
    bool changed(const std::string& name) final;
};

} // flappy
