#pragma once

#include <lib_manager/LibInterface.hpp>
#include <osg_text/TextInterface.hpp>
#include <string>

namespace osg_text
{

    class TextFactoryInterface : public lib_manager::LibInterface
    {

    public:
        TextFactoryInterface(lib_manager::LibManager *theManager) :
            lib_manager::LibInterface(theManager) {}

        virtual ~TextFactoryInterface() {}

        CREATE_MODULE_INFO();
        // LibInterface methods
        int getLibVersion() const {return 1;}
        const std::string getLibName() const {return "osg_text_factory";}

        virtual TextInterface* createText(std::string text="", double fontSize=12,
                                          Color textColor=Color(),
                                          double posX=0., double posY=0.,
                                          TextAlign textAlign=ALIGN_LEFT,
                                          double paddingL=0., double paddingT=0.,
                                          double paddingR=0., double paddingB=0.,
                                          Color backgroundColor=Color(),
                                          Color borderColor=Color(),
                                          double borderWidth = 0.0) = 0;
    };

} // end of namespace: osg_text
