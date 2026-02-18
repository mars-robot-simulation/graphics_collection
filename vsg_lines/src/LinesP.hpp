#pragma once

#include "Lines.hpp"

#include <vsg/all.h>

namespace vsg_lines
{

    class LinesP : public Lines
    {

    public:
        LinesP();
        ~LinesP();

        void appendData(Vector v, Color c = {1.0, 1.0, 1.0, 1.0}) override;
        void clearData();
        void setData(const std::list<Vector> &points) override;
        void setData(const std::list<Vector> &points, const std::list<Color> &colors_) override;
        void drawStrip(bool strip=true) override;
        void setColor(Color c) override;
        void setLineWidth(double w) override;
        void dirty(void);
        void* getVSGNode() override;
        void setBezierMode(bool bezier) override;
        void setBezierInterpolationPoints(int numPoints) override;

    private:
        bool strip, bezierMode;
        int bezierInterpolationPoints;
        std::vector<vsg::vec3> vertices;
        std::vector<vsg::vec3> normals;
        std::vector<vsg::vec2> texcoords;
        std::vector<vsg::vec4> colors;
        vsg::ref_ptr<vsg::vec3Array> points, origPoints;
        vsg::ref_ptr<vsg::MatrixTransform> linesTransform;
        vsg::ref_ptr<vsg::StateGroup> stateGroup;
        vsg::ref_ptr<vsg::Group> node;

        vsg::vec3 getBezierPoint(float t);
    };

} // end of namespace: vsg_lines
