#pragma once

#include <memory>
#include <vector>

#include <IResFactory.h>
#include <GLShaderProgram.h>

namespace flappy {

class GLShaderResFactory: public ResFactory<GLShaderProgram> {
public:
    GLShaderResFactory();

    std::shared_ptr<Res> load(const std::string& name) override;
    std::shared_ptr<Res> create(const std::string& name) override;
};

} // flappy