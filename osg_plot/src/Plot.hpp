#pragma once

#include "Curve.hpp"

#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osgText/Text>

#include <list>

namespace osg_plot
{

    class CurveP;

    class Plot : public osg::Group
    {

    public:
        Plot();
        ~Plot();

        Curve* createCurve(void);
        void removeCurve(Curve*);

        void update(void);

    private:
        int numXTicks, numYTicks;
        float xTicksDiff, yTicksDiff;

        osg::ref_ptr<osg::Vec3Array> xLines;
        osg::ref_ptr<osg::Vec3Array> background;
        osg::ref_ptr<osg::Geometry> bGeom;
        osg::ref_ptr<osg::Geometry> xGeom;
        osg::ref_ptr<osg::DrawArrays> xDrawArray;
        std::list< osg::ref_ptr<CurveP> > curves;

        std::list< osg::ref_ptr<osgText::Text> > xLabels;
        std::list< osg::ref_ptr<osgText::Text> > yLabels;

    };

} // end of namespace: osg_plot
