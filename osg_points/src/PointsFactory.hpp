#pragma once

#include "Points.hpp"

namespace osg_points
{

    class PointsP;

    class PointsFactory
    {

    public:
        PointsFactory();
        ~PointsFactory();

        Points* createPoints(void);

    };

} // end of namespace: osg_points
