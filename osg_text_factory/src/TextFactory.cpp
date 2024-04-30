#include "TextFactory.hpp"

namespace osg_text
{
  
    TextFactory::TextFactory(lib_manager::LibManager *theManager) :
        TextFactoryInterface(theManager)
    {
    }

    TextFactory::~TextFactory(void)
    {
    }

    TextInterface* TextFactory::createText(std::string text, double fontSize,
                                           Color textColor,
                                           double posX, double posY,
                                           TextAlign textAlign,
                                           double paddingL, double paddingT,
                                           double paddingR, double paddingB,
                                           Color backgroundColor,
                                           Color borderColor,
                                           double borderWidth)
    {
        return new Text(text, fontSize, textColor, posX, posY, textAlign,
                        paddingL, paddingT, paddingR, paddingB,
                        backgroundColor, borderColor, borderWidth);
    }

} // end of namespace: osg_text

DESTROY_LIB(osg_text::TextFactory);
CREATE_LIB(osg_text::TextFactory);
