#pragma once

#include "Frame.hpp"

namespace vsg_frames
{

    class FramesP;

    class FramesFactory
    {

    public:
        FramesFactory();
        ~FramesFactory();

        Frame* createFrame(void);
    };

} // end of namespace: vsg_frames
