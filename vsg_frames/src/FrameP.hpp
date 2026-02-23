#pragma once

#include "Frame.hpp"

#include <vsg/all.h>

namespace vsg_frames
{

    class FrameP : public Frame
    {

    public:
        FrameP();
        ~FrameP();

        virtual void setPosition(double x, double y, double z);
        virtual void setRotation(double x, double y, double z, double w);
        virtual void setScale(double x);
        virtual void* getVSGNode();
        virtual void setNodeMask(int mask);

        static vsg::ref_ptr<vsg::Node> frameNode;

    private:
        vsg::ref_ptr<vsg::MatrixTransform> poseTransform;
        vsg::ref_ptr<vsg::MatrixTransform> scaleTransform;
        //vsg::ref_ptr<vsg::StateGroup> stateGroup;
        //vsg::ref_ptr<vsg::Group> node;
        vsg::dvec3 pos;
        vsg::dquat q;
    };

} // end of namespace: vsg_frames
