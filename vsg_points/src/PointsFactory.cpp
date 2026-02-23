#include "PointsFactory.hpp"
#include "PointsP.hpp"

namespace vsg_points
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

} // end of namespace: vsg_points
