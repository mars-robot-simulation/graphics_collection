#include "FramesFactory.hpp"
#include "FrameP.hpp"

#include <vsgXchange/all.h>

#define MACRO_STR(name) #name
#define MACRO_STR2(s) MACRO_STR(s)
#define SHARE_DIR_STR MACRO_STR2(SHARE_DIR)

namespace vsg_frames
{

    FramesFactory::FramesFactory()
    {
        // load frames resource
        std::string s = SHARE_DIR_STR;
        s = s.substr(1, s.size() - 2);
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
