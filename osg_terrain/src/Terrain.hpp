#pragma once

#include <osgDB/ReadFile>
#include <osg/Group>
#include <osg/Geometry>
#include <osg_material_manager/OsgMaterialManager.hpp>
#include <osg_material_manager/MaterialNode.hpp>
#include <mars_utils/Vector.h>
#include "VertexBufferTerrain.hpp"

namespace osg_terrain
{

    osg::ref_ptr<osg::Node> readBobjFromFile(const std::string &filename);

    class Terrain : public osg::Group
    {

    public:
        Terrain(osg_material_manager::OsgMaterialManager *m);
        ~Terrain();
        void setCameraPos(double x, double y, double z);

    private:
        osg_material_manager::OsgMaterialManager *materialManager;
        osg::ref_ptr<osg::Node> createPlane();
        osg::ref_ptr<VertexBufferTerrain> vbt;
        double tPosX, tPosY;
    };

} // end of namespace: osg_terrain
