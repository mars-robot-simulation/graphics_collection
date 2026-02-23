#pragma once

#include "Points.hpp"

#include <vsg/all.h>

namespace vsg_points
{

    class PointsP : public Points
    {

    public:
        PointsP();
        ~PointsP();

        void appendData(Vector v) override;
        void clearData();
        void setData(const std::list<Vector> &points) override;
        void setColor(Color c) override;
        void setColors(const std::vector<Color> &colors) override;
        void setLineWidth(double w) override;
        void dirty(void);
        void* getVSGNode() override;

    private:
        Color color;
        double pointsize;
        std::vector<vsg::vec3> vertices;
        std::vector<vsg::vec4> colors;
        vsg::ref_ptr<vsg::StateGroup> stateGroup;
        vsg::ref_ptr<vsg::Group> node;
    };

} // end of namespace: vsg_points
