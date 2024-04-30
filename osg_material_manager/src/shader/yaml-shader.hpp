#pragma once

#include "shader-function.hpp"
#include <configmaps/ConfigData.h>

#include <vector>
#include <string>

namespace osg_material_manager
{

    class YamlShader : public ShaderFunc
    {
    public:
        YamlShader(std::string name, std::vector<std::string> &args, configmaps::ConfigMap &map, std::string resPath);
        std::string code() const;
    private:
        std::string source;
    };
} // end of namespace osg_material_manager
