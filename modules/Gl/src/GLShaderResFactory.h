#pragma once

#include <memory>
#include <vector>

#include <ResFactory.h>
#include <GLShaderRes.h>

namespace flappy {

class GLShaderResFactory: public ResFactory<GLShaderRes> {
public:
    GLShaderResFactory();

    std::shared_ptr<ResBase> load(const std::string& name) final;
    std::shared_ptr<ResBase> create(const std::string& name) final;
    bool changed(const std::string& name) final;
};

} // flappy
