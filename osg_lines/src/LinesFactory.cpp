#include "LinesFactory.hpp"
#include "LinesP.hpp"

namespace osg_lines
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

} // end of namespace: osg_lines
