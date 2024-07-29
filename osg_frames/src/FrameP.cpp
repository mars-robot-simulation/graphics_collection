#include "FrameP.hpp"

namespace osg_frames
{

    osg::ref_ptr<osg::Node> FrameP::frameNode = nullptr;

  
    FrameP::FrameP() :
        poseTransform{new osg::PositionAttitudeTransform},
        scaleTransform{new osg::MatrixTransform}
    {
        scaleTransform->addChild(FrameP::frameNode);
        poseTransform->addChild(scaleTransform);
    }

    FrameP::~FrameP(void)
    {
    }

    void FrameP::setPosition(double x, double y, double z)
    {
        poseTransform->setPosition(osg::Vec3{x, y, z});
    }

    void FrameP::setRotation(double x, double y, double z, double w)
    {
        poseTransform->setAttitude(osg::Quat{x, y, z, w});
    }

    void FrameP::setScale(double x)
    {
        scaleTransform->setMatrix(osg::Matrix::scale(x, x, x));
    }
  
    void* FrameP::getOSGNode()
    {
        return static_cast<void*>(poseTransform.get());
    }

} // end of namespace: osg_frames
