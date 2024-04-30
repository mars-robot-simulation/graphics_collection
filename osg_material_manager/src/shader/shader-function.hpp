#pragma once

#include <cstdio>
#include <set>
#include <list>
#include <map>
#include <string>
#include <vector>
#include "shader-types.hpp"

namespace osg_material_manager
{

    class ShaderFunc
    {
    public:
        ShaderFunc(std::string name, std::vector<std::string> args, unsigned int priority=0)
            {
                funcs.push_back(FunctionCall(name, args, priority, 0));
                this->name = name;
                // minimum gl version
                minVersion = 120;
                shaderCode = code();
            }

        ShaderFunc()
            {
                this->name = "";
                // minimum gl version
                minVersion = 120;
                shaderCode = code();
            }

        virtual ~ShaderFunc() {}

        void setMinVersion(int minVersion)
            {
                if (minVersion>this->minVersion)
                    this->minVersion = minVersion;
            }

        int getMinVersion()
            {
                return minVersion;
            }

        void addDependencyCode(std::string codeId, std::string code)
            {
                deps[codeId] = code;
            }

        std::vector< std::pair<std::string,std::string> > getDeps() const
            {
                std::vector< std::pair<std::string,std::string> > v;
                v.reserve(deps.size());
                v.insert(v.begin(), deps.begin(), deps.end());
                return v;
            }

        void addVarying(GLSLVarying varying)
            {
                varyings.insert(varying);
            }

        const std::set<GLSLVarying>& getVaryings() const
            {
                return varyings;
            }

        void addUniform(GLSLUniform uniform)
            {
                uniforms.insert(uniform);
            }
      
        const std::set<GLSLUniform>& getUniforms() const
            {
                return uniforms;
            }

        void addConstant(GLSLConstant constant)
            {
                constants.insert(constant);
            }
        const std::set<GLSLConstant>& getConstants() const
            {
                return constants;
            }

        void addAttribute(GLSLAttribute att)
            {
                attributes.insert(att);
            }
        const std::set<GLSLAttribute>& getAttributes() const
            {
                return attributes;
            }

        void enableExtension(std::string extensionName)
            {
                enabledExtensions.insert(extensionName);
            }
        const std::set<std::string>& getEnabledExtensions() const
            {
                return enabledExtensions;
            }

        void disableExtension(std::string extensionName)
            {
                disabledExtensions.insert(extensionName);
            }
        const std::set<std::string>& getDisabledExtensions() const
            {
                return disabledExtensions;
            }

        void addMainVar(GLSLVariable var, int priority=0)
            {
                mainVars.push_back(MainVar(var.name, var.type, var.value, priority, mainVars.size()));
                addMainVarDec((GLSLAttribute) {var.type, var.name});
            }
        const std::list<MainVar>& getMainVars() const
            {
                return mainVars;
            }

        void addMainVarDec(GLSLAttribute att)
            {
                mainVarDecs.insert(att);
            }

        const std::set<GLSLAttribute>& getMainVarDecs() const
            {
                return mainVarDecs;
            }

        void addSuffix(GLSLSuffix suffix)
            {
                suffixes.insert(suffix);
            }
        const std::set<GLSLSuffix>& getSuffixes() const
            {
                return suffixes;
            }

        void addExport(GLSLExport e)
            {
                exports.push_back(e);
            }
        const std::vector<GLSLExport>& getExports() const
            {
                return exports;
            }

        void addSnippet(std::string line, int priority=0)
            {
                snippets.push_back(PrioritizedLine(line, priority, snippets.size()));
            }

        const std::vector<PrioritizedLine>& getSnippets() const
            {
                return snippets;
            }

        const std::vector<FunctionCall>& getFunctionCalls() const
            {
                return funcs;
            }

        std::string generateFunctionCode()
            {
                return code() + "\n" + shaderCode;
            }

        virtual std::string code() const
            {
                return "";
            }

        void merge(ShaderFunc *u);

        std::vector<std::string> generateFunctionCall();

    protected:
        std::vector<FunctionCall> funcs;
        std::string shaderCode;
        std::string name;
        // user variables
        std::set<GLSLUniform> uniforms;
        //
        std::set<GLSLConstant> constants;
        // passes calculations from vertex to fragment shader
        std::set<GLSLVarying> varyings;
        // per vertex attributes
        std::set<GLSLAttribute> attributes;
        // needed functions (tuple of name and code)
        std::map<std::string,std::string> deps;
        std::list<MainVar> mainVars;
        std::set<GLSLAttribute> mainVarDecs;
        std::vector<GLSLExport> exports;
        std::set<GLSLSuffix> suffixes;
        std::set<std::string> enabledExtensions;
        std::set<std::string> disabledExtensions;
        std::vector<PrioritizedLine> snippets;
        // minimum gl version
        int minVersion;

    private:
        static bool mainVarDecs_unique_pred(GLSLAttribute &first, GLSLAttribute &second)
            {
                return first.name == second.name;
            }

    }; // end of class ShaderFunc

} // end of namespace osg_material_manager
