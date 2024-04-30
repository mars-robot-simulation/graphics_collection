#pragma once

#include <string>
#include <ostream>
#include <vector>

namespace osg_material_manager
{

    typedef struct
    {
        std::string type;
        std::string name;
    } GLSLAttribute;

    typedef GLSLAttribute GLSLVarying;
    typedef GLSLAttribute GLSLUniform;
    typedef GLSLAttribute GLSLSuffix;

    typedef struct
    {
        std::string name;
        std::string value;
    } GLSLExport;

    typedef struct
    {
        std::string type;
        std::string name;
        std::string value;
    } GLSLVariable;

    typedef GLSLVariable GLSLConstant;


    struct PrioritizedValue
    {
        int priority;
        int s_priority; // used to preserve order in case of same priority
        bool operator<(const PrioritizedValue& other) const
            {
                if (priority == other.priority)
                {
                    return s_priority > other.s_priority;
                }
                return priority > other.priority;
            };
    };

    struct FunctionCall : PrioritizedValue
    {
        std::string name;
        std::vector<std::string> arguments;
        FunctionCall(std::string name, std::vector<std::string> args, int prio, int s_prio) : name(name), arguments(args)
            {
                priority = prio;
            }
        std::string toString() const
            {
                std::string call = name + "( ";
                unsigned long numArgs = arguments.size();

                if(numArgs > 0)
                {
                    call += arguments[0];
                    for(unsigned long i=1; i<numArgs; ++i)
                    {
                        call += ", " + arguments[i];
                    }
                }
                call += " )";
                return call;
            }
    };

    typedef struct MainVar : PrioritizedValue, GLSLVariable
    {
        MainVar(std::string p_name, std::string p_type, std::string p_value, int p_priority, int p_s_priority)
            {
                priority = p_priority;
                s_priority = p_s_priority;
                name = p_name;
                value = p_value;
                type = p_type;
            }
        std::string toString() const
            {
                return name + " = " + value;
            }
    } MainVar;

    struct PrioritizedLine : PrioritizedValue
    {
        std::string line;
        PrioritizedLine(std::string p_line, int p_priority, int p_s_priority) : line(p_line)
            {
                priority = p_priority;
                s_priority = p_s_priority;
            }
    };

    std::ostream& operator<<(std::ostream& os, const GLSLAttribute& a);
    std::ostream& operator<<(std::ostream& os, const GLSLExport& a);
    std::ostream& operator<<(std::ostream& os, const GLSLVariable& a);
    bool operator<(const GLSLAttribute& a, const GLSLAttribute& b);
    bool operator<(const GLSLExport& a, const GLSLExport& b);
    bool operator<(const GLSLVariable& a, const GLSLVariable& b);

} // end of namespace osg_material_manager

