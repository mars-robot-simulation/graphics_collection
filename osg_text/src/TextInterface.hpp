#pragma once

#include <string>

namespace osg_text
{

    class Color
    {
    public:
        Color() : r(1.0), g(1.0), b(1.0), a(1.0)
            {
            }
        Color(double _r, double _g, double _b, double _a) :
            r(_r), g(_g), b(_b), a(_a)
            {
            }
        double r, g, b, a;
    };

    enum TextAlign
    {
        ALIGN_LEFT,
        ALIGN_CENTER,
        ALIGN_RIGHT
    };

    class TextInterface
    {

    public:
        TextInterface() {}

        virtual ~TextInterface() {}
        virtual void setText(const std::string &s) = 0;
        virtual void setBackgroundColor(const Color &c) = 0;
        virtual void setBorderColor(const Color &c) = 0;
        virtual void setBorderWidth(double w) = 0;
        virtual void setPadding(double left, double top, double right,
                                double bottom) = 0;
        virtual void* getOSGNode() = 0;
        virtual void setFixedWidth(double w) = 0;
        virtual void setFixedHeight(double h) = 0;
        virtual void setPosition(double x, double y) = 0;
        virtual void getPosition(double *x, double *y) = 0;
        virtual std::string getText() = 0;
        virtual void setFontResolution(int x, int y) = 0;
        virtual void getRectangle(double *left, double *right,
                                  double *top, double *bottom) = 0;
        virtual double getFontsize() = 0;
        virtual void getPadding(double *pl, double *pt, double *pr, double *pb) = 0;
        virtual TextAlign getAlign() = 0;
        virtual Color getBackgroundColor() = 0;
        virtual Color getBorderColor() = 0;
        virtual double getBorderWidth() = 0;
        virtual std::string getFont() = 0;
    };

} // end of namespace: osg_text
