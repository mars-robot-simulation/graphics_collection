#pragma once

#include "Frame.hpp"

namespace osg_frames
{

    class FramesP;

    class FramesFactory
    {

    public:
        FramesFactory();
        ~FramesFactory();

        Frame* createFrame(void);
    };

} // end of namespace: osg_frames
