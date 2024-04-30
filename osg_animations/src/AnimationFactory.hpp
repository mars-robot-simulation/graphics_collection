#pragma once

#include "Animation.hpp"

namespace osg_animation
{

    class AnimationP;

    class AnimationFactory
    {

    public:
        AnimationFactory();
        ~AnimationFactory();

        Animation* createAnimation(void);
    };

} // end of namespace: osg_animation
