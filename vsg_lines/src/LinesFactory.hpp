#pragma once

#include "Lines.hpp"

namespace vsg_lines
{

    class LinesP;

    class LinesFactory
    {

    public:
        LinesFactory();
        ~LinesFactory();

        Lines* createLines(void);

    };

} // end of namespace: vsg_lines

