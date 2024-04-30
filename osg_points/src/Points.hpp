#pragma once

#include <vector>

namespace osg_points
{

    struct Vector
    {
        Vector(double x, double y, double z) : x(x), y(y), z(z) {}
        double x, y, z;
    };

    struct Color
    {
        Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
        float r, g, b, a;
    };

    class Points
    {

    public:
        Points() {}
        virtual ~Points() {}

        virtual void appendData(Vector v) = 0;
        virtual void setData(const std::vector<Vector> &points) = 0;
        virtual void setColor(Color c) = 0;
        virtual void setColors(const std::vector<Color> &colors) = 0;
        virtual void setLineWidth(double w) = 0;
        virtual void* getOSGNode() = 0;
    };

} // end of namespace: osg_points
