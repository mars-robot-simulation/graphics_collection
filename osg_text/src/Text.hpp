#pragma once
#include "TextInterface.hpp"

#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osgText/Text>

#include <string>

namespace osg_text
{

    class Text : public TextInterface, public osg::Group
    {

    public:
        Text(std::string text="", double fontSize=12, Color textColor=Color(),
             double posX=0., double posY=0., TextAlign textAlign=ALIGN_LEFT,
             double paddingL=0., double paddingT=0.,
             double paddingR=0., double paddingB=0.,         
             Color backgroundColor=Color(),
             Color borderColor=Color(),
             double borderWidth = 0.0, std::string fontPath="");

        ~Text();
        void setText(const std::string &s);
        void setFontSize(const double fontSize);
        void setBackgroundColor(const Color &c);
        void setBorderColor(const Color &c);
        void setBorderWidth(double w);
        void setPadding(double left, double top, double right, double bottom);
        void* getOSGNode();
        void setFixedWidth(double w);
        void setFixedHeight(double h);
        void setPosition(double x, double y);
        void setFontResolution(int x, int y);
        void getRectangle(double *left, double *right,
                          double *top, double *bottom);
        void getPosition(double *x, double *y);
        std::string getText();
        double getFontsize();
        void getPadding(double *pl, double *pt, double *pr, double *pb);
        TextAlign getAlign();
        Color getBackgroundColor();
        Color getBorderColor();
        double getBorderWidth();
        std::string getFont();

    private:
        osg::ref_ptr<osg::PositionAttitudeTransform> transform;
        osg::ref_ptr<osgText::Text> labelText;
        osg::ref_ptr<osg::Geode> labelGeode;
        osg::ref_ptr<osg::Geode> backgroundGeode;
        osg::ref_ptr<osg::Geode> borderGeode;
        osg::ref_ptr<osg::Geometry> backgroundGeom;
        osg::ref_ptr<osg::Geometry> borderGeom;
        osg::ref_ptr<osg::Vec3Array> backgroundVertices;
        osg::ref_ptr<osg::Vec3Array> borderVertices;
        osg::ref_ptr<osg::MatrixTransform> resolutionCorrection;

        double width, height;
        double posX, posXI, posY;
        TextAlign textAlign;
        double pl, pt, pr, pb;
        double fixedWidth, fixedHeight, w, h, posXB;
        double fontSize;
        osg::ref_ptr<osg::Vec4Array> backgroundColor, borderColor;
        double borderWidth;
        double resolutionCorrectionX, resolutionCorrectionY;
        std::string text_;
        std::string font;

        void updateSize();
        void updateBoundingBox();
        void updatePosition();
        void createBackground(Color bgColor);
        void updateBackgroundPos();
        void createBorder(Color bColor);
        void updateBorderPos();
    };

} // end of namespace: osg_text
