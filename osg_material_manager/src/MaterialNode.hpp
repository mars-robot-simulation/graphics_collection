#pragma once

#include <mars_utils/Vector.h>
#include <mars_interfaces/LightData.h>

#include <vector>

#include <osg/Group>
#include <osg/Geometry>
#include <osg/BlendEquation>
#include <osg/Uniform>
#include <osg/Depth>

namespace osg_material_manager
{
    class OsgMaterial;

    class TangentVisitor: public osg::NodeVisitor
    {
    public:
        TangentVisitor() : osg::NodeVisitor(TRAVERSE_ALL_CHILDREN) {}
        virtual void apply(osg::Node &searchNode);
    private:
        void generateTangents(osg::Geometry *geom);
    };

    class PositionVisitor: public osg::NodeVisitor
    {
    public:
        PositionVisitor() : osg::NodeVisitor(TRAVERSE_ALL_CHILDREN),
                            pos(mars::utils::Vector(0, 0, 0)) { }
        virtual void apply(osg::Node &searchNode);
        mars::utils::Vector pos;
    };

    class InstancesVisitor: public osg::NodeVisitor
    {
    public:
        InstancesVisitor() : osg::NodeVisitor(TRAVERSE_ALL_CHILDREN) {}
        virtual void apply(osg::Node &searchNode);
        int numInstances;
        double width, length, height;
    private:
        void enableInstancing(osg::Geometry *geom);
    };

    class MaterialNode : public osg::Group
    {
    public:
        MaterialNode();
        virtual ~MaterialNode();

        void createNodeState();
        void setMaterial(OsgMaterial *m);
        osg::ref_ptr<OsgMaterial> getMaterial();
        // can be used for dynamic textures
        virtual void setBlending(bool mode);

        void updateLights(std::vector<mars::interfaces::LightData*> &lightList);

        void getNodePosition(mars::utils::Vector *pos);
        bool addChild(osg::Node *child);

        void setBrightness(float val);
        void setUseFog(bool val);
        void setUseNoise(bool val);
        void setNoiseAmmount(float val);
        void setDrawLineLaser(bool val);
        void setUseShadow(bool val);

        /**
         * Sets the line laser
         * @pos: position of the laser
         * @normal: normalvector of the laser-plane
         * @color: color of the laser in RGB
         * @laser: Angle of the laser, as an direction-vector
         * @openingAngle: Opening angle of the laser; for complete laserLine, choose PI
         */
        void setExperimentalLineLaser(mars::utils::Vector lineLasePos,
                                      mars::utils::Vector lineLaserNormal,
                                      mars::utils::Vector lineLaserColor,
                                      mars::utils::Vector LaserAngle,
                                      float openingAngle);

        void setMaxNumLights(int n) {maxNumLights = n;}
        void seperateMaterial();
        void setTransparency(float t);
        void enableInstancing();
        void setNeedInstancing(bool v, int numInstances, double w=1.0, double h=1.0, double l=1.0);
        void generateTangents();
        void setNeedTangents(bool v);
        void setRenderBin(int r) {renderBin = r;}

    protected:
        bool isCreated;
        bool useFog, useNoise;
        bool getLight;
        osg::ref_ptr<osg::Uniform> lightPosUniform, lightAmbientUniform, lightDiffuseUniform;
        osg::ref_ptr<osg::Uniform> lightSpecularUniform, lightIsSpotUniform;
        osg::ref_ptr<osg::Uniform> lightSpotDirUniform, lightIsDirectionalUniform;
        osg::ref_ptr<osg::Uniform> lightConstantAttUniform, lightLinearAttUniform, lightQuadraticAttUniform;
        osg::ref_ptr<osg::Uniform> lightIsSetUniform, lightCosCutoffUniform, lightSpotExponentUniform;


        osg::ref_ptr<OsgMaterial> material;
        osg::ref_ptr<osg::BlendEquation> blendEquation_;
        osg::ref_ptr<osg::Uniform> brightnessUniform;
        osg::ref_ptr<osg::Uniform> transparencyUniform;
        osg::ref_ptr<osg::Uniform> lineLaserPosUniform;
        osg::ref_ptr<osg::Uniform> lineLaserNormalUniform;
        osg::ref_ptr<osg::Uniform> lineLaserColor;
        osg::ref_ptr<osg::Uniform> lineLaserDirection;
        osg::ref_ptr<osg::Uniform> lineLaserOpeningAngle;
        osg::ref_ptr<osg::Uniform> useFogUniform, useNoiseUniform, noiseAmmountUniform;
        osg::ref_ptr<osg::Uniform> useShadowUniform;
        osg::ref_ptr<osg::Uniform> numLightsUniform;
        osg::ref_ptr<osg::Uniform> drawLineLaserUniform;
        osg::ref_ptr<osg::StateSet> materialState;
        osg::ref_ptr<osg::Depth> depth;

        bool drawLineLaser;
        bool shadow;
        int maxNumLights;
        bool needTangents;
        bool needInstancing;
        int numInstances;
        double iWidth, iHeight, iLength;
        int renderBin;
        mars::utils::Vector lineLasePos, lineLaserNormal;
        std::vector<std::string> filterLights;
    }; // end of class MaterialNode

} // end of namespace osg_material_manager
