#pragma once

#include "OsgMaterial.hpp"

#include <lib_manager/LibInterface.hpp>
#include <cfg_manager/CFGManagerInterface.h>
#include <cfg_manager/CFGClient.h>
#include <mars_interfaces/LightData.h>

namespace osg_material_manager
{

    class OsgMaterialManager : public lib_manager::LibInterface,
                               public mars::cfg_manager::CFGClient
    {

        struct textureFileStruct
        {
            std::string fileName;
            osg::ref_ptr<osg::Texture2D> texture;
        }; // end of struct textureFileStruct

        struct imageFileStruct
        {
            std::string fileName;
            osg::ref_ptr<osg::Image> image;
        }; // end of struct imageFileStruct

    public:
        OsgMaterialManager(lib_manager::LibManager *theManager);
        OsgMaterialManager(const std::string &resourcesPath);
        void init();
        ~OsgMaterialManager();


        CREATE_MODULE_INFO();
        // LibInterface methods
        int getLibVersion() const {return 1;}
        const std::string getLibName() const {return "osg_material_manager";}

        virtual void cfgUpdateProperty(mars::cfg_manager::cfgPropertyStruct _property);

        void createMaterial(const std::string &name,
                            const configmaps::ConfigMap &map);
        void setMaterial(const std::string &name,
                         const configmaps::ConfigMap &map);
        void editMaterial(const std::string &name, const std::string &key,
                          const std::string &value);
        osg::ref_ptr<OsgMaterial> getOsgMaterial(const std::string &name);
        osg::ref_ptr<MaterialNode> getNewMaterialGroup(const std::string &name);
        void removeMaterialGroup(osg::ref_ptr<osg::Group> group);
        void setShadowSamples(int v);

        osg::ref_ptr<osg::Group> getMainStateGroup() {return mainStateGroup.get();}
        void updateLights(std::vector<mars::interfaces::LightData*> &lightList);

        // setter for global states
        void setUseShader(bool v);
        bool getUseShader() const;
        void setShadowTextureSize(int size);
        void setShadowScale(float v);
        void setDefaultMaxNumLights(int v);
        void setUseFog(bool v);
        void setUseNoise(bool v);
        void setDrawLineLaser(bool v);
        void setUseShadow(bool v);
        void setShadowTechnique(std::string v);
        void setBrightness(float v);
        void setNoiseAmmount(float v);

        std::vector<configmaps::ConfigMap> getMaterialList();
        void setExperimentalLineLaser(mars::utils::Vector pos,
                                      mars::utils::Vector normal,
                                      mars::utils::Vector color,
                                      mars::utils::Vector laserAngle,
                                      float openingAngle);
        void updateShadowSamples();

        static osg::ref_ptr<osg::TextureCubeMap> loadCubemap(configmaps::ConfigMap &info, std::string loadPath="");
        static osg::ref_ptr<osg::Texture2D> loadTexture(std::string filename);
        static osg::ref_ptr<osg::Image> loadImage(std::string filename);

    private:
        mars::cfg_manager::CFGManagerInterface *cfg;
        osg::ref_ptr<osg::Group> mainStateGroup;
        osg::ref_ptr<osg::Image> noiseImage;
        mars::cfg_manager::cfgPropertyStruct resPath, shadowSamples;
        std::map<std::string, osg::ref_ptr<OsgMaterial> > materialMap;
        std::vector<osg::ref_ptr<MaterialNode> > materialNodes;

        // most properties are currently global settings
        // global OsgMaterial properties
        bool useShader;
        int shadowTextureSize, defaultMaxNumNodeLights;
        float shadowScale;
        // global MaterialNode properties
        bool useFog, useNoise, drawLineLaser, useShadow;
        float brightness, noiseAmmount;
        std::string shadowTechnique;

        static std::vector<textureFileStruct> textureFiles;
        static std::vector<imageFileStruct> imageFiles;
        static std::map<std::string,osg::ref_ptr<osg::TextureCubeMap>> cubemaps;
    };

} // end of namespace: osg_material_manager
