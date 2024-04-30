#pragma once

#include <string>

namespace osg_plot
{

  class Curve
  {

  public:
    Curve() {}
    virtual ~Curve() {}

    virtual void setMaxNumPoints(unsigned long n) = 0;
    virtual void setTitle(std::string s) = 0;
    virtual void appendData(double x, double y) = 0;
    virtual void setYBounds(double yMin, double yMax) = 0;
  };

} // end of namespace: osg_plot
