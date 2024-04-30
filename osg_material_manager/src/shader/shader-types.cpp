#include "shader-types.hpp"

namespace osg_material_manager
{

    using namespace std;

    ostream& operator<<(ostream& os, const GLSLExport& a)
    {
        return os << a.name << " = " << a.value;
    }
    bool operator<(const GLSLExport& a, const GLSLExport& b)
    {
        return a.name < b.name;
    }

    ostream& operator<<(ostream& os, const GLSLAttribute& a)
    {
        return os << a.type << " " << a.name;
    }
    bool operator<(const GLSLAttribute& a, const GLSLAttribute& b)
    {
        return a.name < b.name;
    }

    ostream& operator<<(ostream& os, const GLSLVariable& a)
    {
        return os << a.type << " " << a.name << " = " << a.value;
    }
    bool operator<(const GLSLVariable& a, const GLSLVariable& b)
    {
        return a.name < b.name;
    }

} // end of namespace osg_material_manager
