#pragma once

#include <set>
#include <vector>
#include "shader-types.hpp"

namespace osg_material_manager
{

    using namespace std;

    /**
     * Abstract class for shader providers. An implementation of this class should suffice to generate valid shader code
     * when used by the ShaderFactory.
     */
    class IShaderProvider
    {
    public:

        IShaderProvider(string res_path)
            {
                resPath = res_path;
            }
        virtual ~IShaderProvider() {}

        /**
         * Returns the minimal glsl version needed for the generated shader
         */
        virtual int getMinVersion() = 0;

        /**
         * Returns the set of uniforms needed by this shader.
         */
        virtual const set<GLSLUniform>& getUniforms() const = 0;

        /**
         * Returns the set of varyings needed/created by this shader
         */
        virtual const set<GLSLAttribute>& getVaryings() const = 0;

        /**
         * Returns the set of extensions enabled by this shader
         */
        virtual const set<string>& getEnabledExtensions() const = 0;

        /**
         * Returns the set of extensions disabled by this shader
         */
        virtual const set<string>& getDisabledExtensions() const = 0;

        /**
         * Returns this shaders dependencies
         */
        virtual const vector<pair<string, string> > getDependencies() const = 0;

        /**
         * Returns the set of constants created by this shader
         */
        virtual const std::set<GLSLConstant>& getConstants() const = 0;

        /**
         * Returns the set of attributes needed by this shader
         */
        virtual const std::set<GLSLAttribute>& getAttributes() const = 0;

        /**
         * Generates the main function of this shader
         */
        virtual string generateMainSource() = 0;

        /**
         * Generates function definitions for this shader.
         * Should also contain everything else outside of the main function that is not defining
         * uniforms, varyings, extensions, constants or attributes.
         */
        virtual string generateDefinitions() = 0;

    protected:
        /**
         * The resource path for accessing definition files
         */
        string resPath;
    };
}
