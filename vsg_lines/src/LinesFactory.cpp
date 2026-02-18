#include "LinesFactory.hpp"
#include "LinesP.hpp"

namespace vsg_lines
{

    LinesFactory::LinesFactory()
    {
    }

    LinesFactory::~LinesFactory(void)
    {
    }

    Lines* LinesFactory::createLines(void)
    {
        LinesP *lines = new LinesP();
        return lines;
    }

} // end of namespace: vsg_lines
