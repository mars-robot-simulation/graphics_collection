#pragma once

#include "Points.hpp"

namespace vsg_points
{

    class PointsP;

    class PointsFactory
    {

    public:
        PointsFactory();
        ~PointsFactory();

        Points* createPoints(void);

    };

} // end of namespace: vsg_points

