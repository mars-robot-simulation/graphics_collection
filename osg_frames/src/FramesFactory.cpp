#include "FramesFactory.hpp"
#include "FrameP.hpp"

#include <osgDB/ReadFile>

namespace osg_frames
{

    FramesFactory::FramesFactory()
    {
        // load frames resource
        std::stringstream ss;
        ss << SHARE_DIR;
        ss << "/resources/frame.obj";
        fprintf(stderr, "osg_frames: load %s\n", ss.str().c_str());
        FrameP::frameNode = osgDB::readNodeFile(ss.str());
    }

    FramesFactory::~FramesFactory(void)
    {
    }

    Frame* FramesFactory::createFrame(void)
    {
        return new FrameP;
    }

} // end of namespace: osg_frames
