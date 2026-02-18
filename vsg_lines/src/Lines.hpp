#pragma once

#include <list>

namespace vsg_lines
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

    class Lines
    {

    public:
        Lines() {}
        virtual ~Lines() {}

        virtual void appendData(Vector v, Color c = {1.0, 1.0, 1.0, 1.0}) = 0;
        virtual void setData(const std::list<Vector> &points) = 0;
        virtual void setData(const std::list<Vector> &points, const std::list<Color> &colors_) = 0;
        virtual void drawStrip(bool strip=true) = 0;
        virtual void setColor(Color c) = 0;
        virtual void setLineWidth(double w) = 0;
        virtual void* getVSGNode() = 0;
        virtual void setBezierMode(bool bezier = true) = 0;
        virtual void setBezierInterpolationPoints(int numPoints = 20) = 0;
    };

} // end of namespace: vsg_lines
