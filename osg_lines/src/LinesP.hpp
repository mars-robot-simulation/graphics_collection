#pragma once

#include "Lines.hpp"

#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osg/LineWidth>

namespace osg_lines
{

    class LinesP : public osg::Group, public Lines
    {

    public:
        LinesP();
        ~LinesP();

        void appendData(Vector v);
        void clearData();
        void setData(const std::list<Vector> &points);
        void drawStrip(bool strip=true);
        void setColor(Color c);
        void setLineWidth(double w);
        void dirty(void);
        void* getOSGNode();
        void setBezierMode(bool bezier);
        void setBezierInterpolationPoints(int numPoints);

    private:
        bool strip, bezierMode;
        int bezierInterpolationPoints;
        osg::ref_ptr<osg::Vec3Array> points, origPoints;
        osg::ref_ptr<osg::Geometry> linesGeom;
        osg::ref_ptr<osg::MatrixTransform> linesTransform;
        osg::ref_ptr<osg::DrawArrays> drawArray;
        osg::ref_ptr<osg::LineWidth> linew;
        osg::ref_ptr<osg::Vec4Array> colors;
        osg::ref_ptr<osg::Geode> node;

        osg::Vec3 getBezierPoint(float t);
    };

} // end of namespace: osg_lines
