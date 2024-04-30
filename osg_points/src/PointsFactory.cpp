#include "PointsFactory.hpp"
#include "PointsP.hpp"

namespace osg_points
{

    PointsFactory::PointsFactory()
    {
    }

    PointsFactory::~PointsFactory(void)
    {
    }

    Points* PointsFactory::createPoints(void)
    {
        PointsP *points = new PointsP();
        return points;
    }

} // end of namespace: osg_points
