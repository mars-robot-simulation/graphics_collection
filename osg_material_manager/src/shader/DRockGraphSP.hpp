#pragma once

#include "IShaderProvider.hpp"
#include <configmaps/ConfigData.h>

namespace osg_material_manager
{

    using namespace std;
    using namespace configmaps;

    class DRockGraphSP : public IShaderProvider
    {
    public:
        DRockGraphSP(string res_path, ConfigMap graph, ConfigMap options, string shadowTechnique="none");

        int getMinVersion();

        const set<GLSLUniform>& getUniforms() const;

        const set<GLSLAttribute>& getVaryings() const;

        const set<string>& getEnabledExtensions() const;

        const set<string>& getDisabledExtensions() const;

        const vector<pair<string, string> > getDependencies() const;

        const set<GLSLConstant>& getConstants() const;

        const set<GLSLAttribute>& getAttributes() const;

        string generateMainSource();

        string generateDefinitions();

    private:
        ConfigMap model;
        /**
         * Containing options needed for shader generation.
         * Fields:
         * num_lights: needed
         */
        ConfigMap options;

        /**
         * Contains the shaders main function generated from the graph definition
         */
        string main_source;

        /**
         * Contains resource path to all source code needed by the main function
         */
        map<string, string> source_files;
        string shadowTechnique;
        int minVersion;
        set<GLSLUniform> uniforms;
        set<GLSLAttribute> varyings;
        set<string> enabledExtensions;
        set<string> disabledExtensions;
        vector<pair<string, string> > dependencies;
        set<GLSLConstant> constants;
        set<GLSLAttribute> attributes;

        /**
         * Parses the graph file to fill the fields with the correct content
         */
        void parseGraph();

        /**
         * Parses a functionInfo and adds varyings, uniforms and function source code to the provider
         */
        void parse_functionInfo(string functionName, ConfigMap functionInfo);
    };
}
