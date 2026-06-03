#include "FramesFactory.hpp"
#include "FrameP.hpp"
#include "config.h"

#include <vsgXchange/all.h>

namespace vsg_frames
{

    FramesFactory::FramesFactory()
    {
        // load frames resource
        std::string s = std::string(SHARE_DIR);
        s += "/resources/frame.obj";
        fprintf(stderr, "osg_frames: load %s\n", s.c_str());
        auto options = vsg::Options::create(vsgXchange::all::create());
        //options->sharedObjects = vsg::SharedObjects::create();
        options->add(vsgXchange::all::create());
        FrameP::frameNode = vsg::read_cast<vsg::Node>(s, options);
    }

    FramesFactory::~FramesFactory(void)
    {
    }

    Frame* FramesFactory::createFrame(void)
    {
        return new FrameP;
    }

} // end of namespace: vsg_frames
