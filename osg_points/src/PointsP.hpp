#pragma once

#include "Points.hpp"

#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osg/Point>

namespace osg_points
{

    class PointsP : public osg::Group, public Points
    {

    public:
        PointsP();
        ~PointsP();

        void appendData(Vector v);
        void setData(const std::vector<Vector> &points);
        void setColor(Color c);
        void setColors(const std::vector<Color> &colors);
        void setLineWidth(double w);
        void dirty(void);
        void* getOSGNode();

    private:
        osg::ref_ptr<osg::Vec3Array> points;
        osg::ref_ptr<osg::Geometry> pointsGeom;
        osg::ref_ptr<osg::MatrixTransform> pointsTransform;
        osg::ref_ptr<osg::DrawArrays> drawArray;
        osg::ref_ptr<osg::Point> linew;
        osg::ref_ptr<osg::Vec4Array> colors;
        osg::ref_ptr<osg::Geode> node;
    };

} // end of namespace: osg_points
