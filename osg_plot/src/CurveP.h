#pragma once

#include "Curve.hpp"

#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osgText/Text>

namespace osg_plot
{

    struct Color
    {
        float r, g, b, a;
    };

    class CurveP : public osg::Group, public Curve
    {
        friend class Plot;

    public:
        CurveP(int c);
        ~CurveP();

        void setMaxNumPoints(unsigned long n) {maxPoints = n;}
        void setTitle(std::string s) {title = s.c_str();}

        void appendData(double x, double y);
        void crop(void);
        void getBounds(double *minX, double *maxX, double *minY, double *maxY);
        void rescale(double minX, double maxX, double minY, double maxY);
        void setYBounds(double yMin, double yMax)
            {
                this->yMin = yMin;
                this->yMax = yMax;
                boundsSet = true;
            }
        void dirty(void);

    private:
        unsigned long maxPoints;
        int color;
        float yPos;
        float yMin, yMax;
        bool boundsSet;
        std::string title;

        Color defColors[6];
        osg::ref_ptr<osg::Vec3Array> points;
        osg::ref_ptr<osg::Geometry> linesGeom;
        osg::ref_ptr<osg::MatrixTransform> curveTransform;
        osg::ref_ptr<osg::DrawArrays> drawArray;

        osg::ref_ptr<osgText::Text> xLabelText;
    };

} // end of namespace: osg_plot
