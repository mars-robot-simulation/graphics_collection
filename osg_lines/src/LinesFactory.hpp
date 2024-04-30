#pragma once

#include "Lines.hpp"

namespace osg_lines
{

    class LinesP;

    class LinesFactory
    {

    public:
        LinesFactory();
        ~LinesFactory();

        Lines* createLines(void);

    };

} // end of namespace: osg_lines

