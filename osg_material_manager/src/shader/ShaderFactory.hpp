#pragma once

#include <map>
#include <memory>
#include <osg/Program>
#include "IShaderProvider.hpp"

namespace osg_material_manager
{

    using namespace std;

    enum ShaderType
    {
        SHADER_TYPE_FRAGMENT,
        SHADER_TYPE_GEOMETRY,
        SHADER_TYPE_VERTEX,
        SHADER_TYPE_FFP
    };

    /**
     * This class is responsible for generating the osg::Program for shading operations.
     * The IShaderProvider provide the necessary information to generate a shader for each ShaderType
     */
    class ShaderFactory
    {
    public:
        /**
         * Sets a shader provider for given shader type
         * The function takes ownership of the provider pointer by storing it inside of an std::unique_ptr!
         * @param provider A pointer to the shader provider
         * @param shader_type The shader type this provider serves
         */
        void setShaderProvider(IShaderProvider *provider, ShaderType shader_type);

        /**
         * Generates the complete source code for a shader program of given type
         * TODO: Only generate if not generated before/no changes
         * @param shader_type The type to generate the shader for
         * @return String containing the shader source
         */
        string generateShaderSource(ShaderType shader_type);

        /**
         * Generates the entire osg::Program containing all shader types served by a provider.
         * @return The ready to use osg::Program
         */
        osg::Program* generateProgram();

    private:
        /**
         * This map contains the current provider for each shader type.
         * Since the provider pointers are stored inside of an unique_ptr the memory management is taken care of.
         */
        map<ShaderType, std::unique_ptr<IShaderProvider> > providers;
    };
}
