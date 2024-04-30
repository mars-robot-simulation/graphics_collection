#pragma once

#include "Frame.hpp"

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Node>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>

namespace osg_frames
{

    class FrameP : public Frame
    {

    public:
        FrameP();
        ~FrameP();

        virtual void setPosition(double x, double y, double z);
        virtual void setRotation(double x, double y, double z, double w);
        virtual void setScale(double x);
        virtual void* getOSGNode();

        static osg::ref_ptr<osg::Node> frameNode;

    private:
        osg::ref_ptr<osg::Geometry> linesGeom;
        osg::ref_ptr<osg::PositionAttitudeTransform> poseTransform;
        osg::ref_ptr<osg::MatrixTransform> scaleTransform;
        osg::ref_ptr<osg::Geode> node;
    };

} // end of namespace: osg_frames
