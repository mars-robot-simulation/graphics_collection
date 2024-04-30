#pragma once

#include <list>

namespace osg_frames
{

    struct Vector
    {
        Vector(double x, double y, double z) : x(x), y(y), z(z) {}
        double x, y, z;
    };

    class Frame
    {

    public:
        Frame() {}
        virtual ~Frame() {}

        virtual void setPosition(double x, double y, double z) = 0;
        virtual void setRotation(double x, double y, double z, double w) = 0;
        virtual void setScale(double x) = 0;
        virtual void* getOSGNode() = 0;
    };

} // end of namespace: osg_frames

