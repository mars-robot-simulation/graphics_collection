#pragma once

#include <configmaps/ConfigData.h>

#include <osg/Group>
#include <osg/Geometry>
#include <osgUtil/CullVisitor>
#include <osg/Transform>

namespace osg_terrain
{
    class PosTransform : public osg::Transform
    {
    public:
        double zOffset;
        double x, y, z;
        bool overridePos;
        // todo: add x and y offsets
        // todo: add fixe position
        virtual bool computeLocalToWorldMatrix(osg::Matrix& matrix,
                                               osg::NodeVisitor* nv) const;
        virtual bool computeWorldToLocalMatrix(osg::Matrix& matrix,
                                               osg::NodeVisitor* nv) const;
    };

    class ShaderTerrain : public osg::Group
    {

        class BoundCallback: public osg::Drawable::ComputeBoundingBoxCallback
        {
        public:
            double size[3];

            BoundCallback(){}
            ~BoundCallback(){}
            osg::BoundingBox computeBound(const osg::Drawable &) const;

        };

        class BoundVisitor: public osg::NodeVisitor
        {
        public:
            BoundVisitor() : osg::NodeVisitor(TRAVERSE_ALL_CHILDREN) {}
            virtual void apply(osg::Node &searchNode);
            osg::ref_ptr<osg::Drawable::ComputeBoundingBoxCallback> s;
        };

    public:
        ShaderTerrain(configmaps::ConfigMap map);
        ~ShaderTerrain();

        /*
         * Override position transform
         */
        void setPos(double x, double y, double z);
        osg::ref_ptr<osg::Node> getNode();

    private:
        osg::ref_ptr<BoundCallback> boundCallback;
        osg::ref_ptr<PosTransform> posTransform;
    };

} // end of namespace: osg_terrain
