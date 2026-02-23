#pragma once

#include <list>

namespace vsg_frames
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
        virtual void* getVSGNode() = 0;
        virtual void setNodeMask(int mask) = 0;
    };

} // end of namespace: vsg_frames

