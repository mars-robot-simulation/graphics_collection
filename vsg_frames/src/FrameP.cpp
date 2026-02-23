#include "FrameP.hpp"

namespace vsg_frames
{

    vsg::ref_ptr<vsg::Node> FrameP::frameNode = nullptr;

  
    FrameP::FrameP()
    {
        poseTransform = vsg::MatrixTransform::create(vsg::translate(vsg::dvec3(0, 0, 0)));
        scaleTransform = vsg::MatrixTransform::create(vsg::scale(vsg::dvec3(1, 1, 1)));
        scaleTransform->addChild(FrameP::frameNode);
        poseTransform->addChild(scaleTransform);
    }

    FrameP::~FrameP(void)
    {
    }

    void FrameP::setPosition(double x, double y, double z)
    {
        pos.x = x;
        pos.y = y;
        pos.z = z;
        poseTransform->matrix = vsg::translate(pos) * vsg::rotate(q);
    }

    void FrameP::setRotation(double x, double y, double z, double w)
    {
        q.x = x;
        q.y = y;
        q.z = z;
        q.w = w;
        poseTransform->matrix = vsg::translate(pos) * vsg::rotate(q);
    }

    void FrameP::setScale(double x)
    {
        scaleTransform->matrix = vsg::scale(vsg::dvec3(x, x, x));
    }
  
    void* FrameP::getVSGNode()
    {
        return (void*)(vsg::Node*)poseTransform.get();
    }

    void FrameP::setNodeMask(int mask)
    {
        //poseTransform->setNodeMask(mask);
    }

} // end of namespace: vsg_frames
