#pragma once

#include <list>
#include <vector>

namespace vsg_points
{

    struct Vector
    {
        Vector(double ix, double iy, double iz) : x(ix), y(iy), z(iz) {}
        double x, y, z;
    };

    struct Color
    {
        Color(float ir, float ig, float ib, float ia) : r(ir), g(ig), b(ib), a(ia) {}
        float r, g, b, a;
    };

    class Points
    {

    public:
        Points() {}
        virtual ~Points() {}

        virtual void appendData(Vector v) = 0;
        virtual void setData(const std::list<Vector> &points) = 0;
        virtual void setColor(Color c) = 0;
        virtual void setColors(const std::vector<Color> &colors) = 0;
        virtual void setLineWidth(double w) = 0;
        virtual void* getVSGNode() = 0;
    };

} // end of namespace: vsg_points
