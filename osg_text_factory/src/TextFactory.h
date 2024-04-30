#pragma once

#include "TextFactoryInterface.hpp"
#include <osg_text/Text.hpp>

#include <string>

namespace osg_text
{

    class TextFactory : public TextFactoryInterface
    {

    public:
        TextFactory(lib_manager::LibManager *theManager);

        ~TextFactory();

        TextInterface* createText(std::string text="", double fontSize=12,
                                  Color textColor=Color(),
                                  double posX=0., double posY=0.,
                                  TextAlign textAlign=ALIGN_LEFT,
                                  double paddingL=0., double paddingT=0.,
                                  double paddingR=0., double paddingB=0.,
                                  Color backgroundColor=Color(),
                                  Color borderColor=Color(),
                                  double borderWidth = 0.0);
    };

} // end of namespace: osg_text
