#include <sstream>
#include <iostream>
#include <fstream>
#include "yaml-shader.hpp"

namespace osg_material_manager
{

    using namespace std;
    using namespace configmaps;

    YamlShader::YamlShader(string name, vector<std::string> &args, ConfigMap &map, string resPath)
        : ShaderFunc(name, args)
    {
        if (map.hasKey("source"))
        {
            string path = resPath+(string)map["source"];
            ifstream t(path.c_str());
            stringstream buffer;
            buffer << t.rdbuf();
            source = buffer.str();
        } else
        {
            source = "";
        }

        if (map.hasKey("priority"))
        {
            funcs[0].priority = (int)map["priority"];
        } else
        {
            funcs[0].priority = 0;
        }

        if (map.hasKey("params"))
        {
            ConfigVector::iterator it = map["params"].begin();
            for (;it!=map["params"].end();it++)
            {
                funcs[0].arguments.push_back(it.base()->getString());
            }
        }

        if (map.hasKey("varyings"))
        {
            ConfigMap::iterator it = map["varyings"].beginMap();
            for (;it!=map["varyings"].endMap();it++)
            {
                string type = it->first;
                std::size_t a_pos = type.find("[]"); // to determine if value is array or not
                ConfigVector::iterator it2 = map["varyings"][type].begin();
                for (;it2!=map["varyings"][type].end();it2++)
                {
                    ConfigItem &item = *it2;
                    std::stringstream s;
                    string typeName; // should contain type without [] if present at the end
                    if (a_pos != string::npos)
                    {
                        typeName = type.substr(0,a_pos);
                        if (item.hasKey("arraySize"))
                        {
                            string size = map["mappings"][(string)item["arraySize"]];
                            s << "[" << size << "]";
                        } else
                        {
                            s << "[1]"; // fallback arraySize of 1
                        }
                    } else
                    {
                        typeName = type;
                        s << "";
                    }
                    addVarying( (GLSLVarying) { typeName, (string)item["name"] + s.str() } );
                }
            }
        }

        if (map.hasKey("uniforms"))
        {
            ConfigMap::iterator it = map["uniforms"].beginMap();
            for (;it!=map["uniforms"].endMap();it++)
            {
                string type = it->first;
                std::size_t a_pos = type.find("[]"); // to determine if value is array or not
                ConfigVector::iterator it2 = map["uniforms"][type].begin();
                for (;it2!=map["uniforms"][type].end();it2++)
                {
                    ConfigItem &item = *it2;
                    std::stringstream s;
                    string typeName; // should contain type without [] if present at the end
                    if (a_pos != string::npos)
                    {
                        typeName = type.substr(0,a_pos);
                        if (item.hasKey("arraySize"))
                        {
                            string size = map["mappings"][(string)item["arraySize"]];
                            s << "[" << size << "]";
                        } else
                        {
                            s << "[1]"; // fallback arraySize of 1
                        }
                    } else
                    {
                        typeName = type;
                        s << "";
                    }
                    std::string name = (string)item["name"] + s.str();
                    if(!map["mappings"].hasKey(name))
                    {
                        addUniform( (GLSLUniform) { typeName, name} );
                    }
                    else
                    {
                        addConstant((GLSLConstant) { typeName, name,  map["mappings"][name].toString()} );
                    }
                }
            }
        }

        if (map.hasKey("attributes"))
        {
            ConfigMap::iterator it = map["attributes"].beginMap();
            for (;it!=map["attributes"].endMap();it++)
            {
                string type = it->first;
                std::size_t a_pos = type.find("[]"); // to determine if value is array or not
                ConfigVector::iterator it2 = map["attributes"][type].begin();
                for (;it2!=map["attributes"][type].end();it2++)
                {
                    ConfigItem &item = *it2;
                    std::stringstream s;
                    string typeName; // should contain type without [] if present at the end
                    if (a_pos != string::npos)
                    {
                        typeName = type.substr(0,a_pos);
                        if (item.hasKey("arraySize"))
                        {
                            string size = map["mappings"][(string)item["arraySize"]];
                            s << "[" << size << "]";
                        } else
                        {
                            s << "[1]"; // fallback arraySize of 1
                        }
                    } else
                    {
                        typeName = type;
                        s << "";
                    }
                    addAttribute( (GLSLAttribute) { typeName, (string)item["name"] + s.str() } );
                }
            }
        }

        if (map.hasKey("exports"))
        {
            ConfigVector::iterator it = map["exports"].begin();
            for (;it!=map["exports"].end();it++)
            {
                ConfigItem &item = *it;
                addExport( (GLSLExport) { (string)item["name"], (string)item["value"] } );
            }
        }

        if (map.hasKey("mainVarDecs"))
        {
            ConfigMap::iterator it = map["mainVarDecs"].beginMap();
            for (;it!=map["mainVarDecs"].endMap();it++)
            {
                string type = it->first;
                std::size_t a_pos = type.find("[]"); // to determine if value is array or not
                ConfigVector::iterator it2 = map["mainVarDecs"][type].begin();
                for (;it2!=map["mainVarDecs"][type].end();it2++)
                {
                    ConfigItem &item = *it2;
                    std::stringstream s;
                    string typeName;
                    if (a_pos != string::npos)
                    {
                        typeName = type.substr(0,a_pos);
                        if (item.hasKey("arraySize"))
                        {
                            string size = map["mappings"][(string)item["arraySize"]];
                            s << "[" << size << "]";
                        } else
                        {
                            s << "[1]"; // fallback arraySize of 1
                        }
                    } else
                    {
                        typeName = type;
                        s << "";
                    }
                    addMainVarDec((GLSLAttribute) {typeName, (string)item["name"]});
                }
            }
        }

        if (map.hasKey("mainVars"))
        {
            ConfigMap::iterator it = map["mainVars"].beginMap();
            for (;it!=map["mainVars"].endMap();it++)
            {
                string type = it->first;
                std::size_t a_pos = type.find("[]"); // to determine if value is array or not
                ConfigVector::iterator it2 = map["mainVars"][type].begin();
                for (;it2!=map["mainVars"][type].end();it2++)
                {
                    ConfigItem &item = *it2;
                    std::stringstream s;
                    string typeName; // should contain type without [] if present at the end
                    if (a_pos != string::npos)
                    {
                        typeName = type.substr(0,a_pos);
                        if (item.hasKey("arraySize"))
                        {
                            string size = map["mappings"][(string)item["arraySize"]];
                            s << "[" << size << "]";
                        } else
                        {
                            s << "[1]"; // fallback arraySize of 1
                        }
                    } else
                    {
                        typeName = type;
                        s << "";
                    }
                    int priority = funcs[0].priority;
                    if (item.hasKey("priority"))
                    {
                        priority = (int)item["priority"];
                    }
                    addMainVar( (GLSLVariable) { typeName, (string)item["name"], (string)item["value"] }, priority);
                }
            }
        }

        if (map.hasKey("snippets"))
        {
            ConfigVector::iterator it = map["snippets"].begin();
            for (;it!=map["snippets"].end();it++)
            {
                ConfigItem &item = *it;
                string snippet = "";
                if (item.hasKey("source"))
                {
                    string path = resPath+(string)item["source"];
                    ifstream t(path.c_str());
                    stringstream buffer;
                    buffer << t.rdbuf();
                    snippet = buffer.str();
                }
                int priority = funcs[0].priority;
                if (item.hasKey("priority"))
                {
                    priority = (int)item["priority"];
                }
                addSnippet(snippet, priority);
            }
        }
    }

    std::string YamlShader::code() const
    {
        return source;
    }

}
