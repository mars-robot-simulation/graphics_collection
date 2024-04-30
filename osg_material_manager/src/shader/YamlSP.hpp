#pragma once

#include <memory>

#include "IShaderProvider.hpp"
#include <configmaps/ConfigData.h>
#include "shader-function.hpp"
#include "ShaderFactory.hpp"

namespace osg_material_manager
{

  using namespace std;
  using namespace configmaps;

  /**
   * Implementation (not much more than a wrapper) of the IShaderProvider for use with the old YamlShader files.
   * Intended only for demonstration and not recommended for productive use anymore.
   * (The shader functions still have to be created manually by code or YamlShader instances)
   */
  class YamlSP : public IShaderProvider
  {
  public:
    YamlSP(string res_path);
    ~YamlSP() {}

    int getMinVersion();

    const set<GLSLUniform>& getUniforms() const;

    const set<GLSLAttribute>& getVaryings() const;

    const set<string>& getEnabledExtensions() const;

    const set<string>& getDisabledExtensions() const;

    const vector<pair<string, string> > getDependencies() const;

    const std::set<GLSLConstant>& getConstants() const;

    const std::set<GLSLAttribute>& getAttributes() const;

    string generateMainSource();

    string generateDefinitions();

    /**
     * Adds a shader function to this Provider or merges it with an existing one if present.
     * This function takes ownership of the given pointer and manages its memory.
     * @param func The function to add/merge to this provider
     */
    void addShaderFunction(ShaderFunc *func);

    /**
     * Sets up uniforms, exports, etc in code needed by the Yaml Shader Files
     * @param shader_type The Shader Type this YamlSP is the provider for
     * @param material The entire material ConfigMap
     * @param has_texture Bool determining if a texture is used in the shader or not
     * @param use_world_tex_coords Determines if to use global texture coordinates or not
     */
    void setupShaderEnv(ShaderType shader_type, ConfigMap material, bool has_texture, bool use_world_tex_coords);

  private:
    /**
     * Pointer to the shader function containing all necessary information to generate the shader code
     */
    set<GLSLUniform> emptyUniforms;
    set<GLSLAttribute> emptyAttributes;
    set<string> emptyStrings;
    std::set<GLSLConstant> emptyConstants;

    unique_ptr<ShaderFunc> function;
  };
}
