#include "LinesP.hpp"

#include <cstdio>

/*
 * todo:
 *       - implement node mask
 *       - implement array update instead of rebuilding the whole pipeline on change
 *       - implement option to change color or vertice dynamically
 *         - flag wether the lines are dynamic
 *
 **/

namespace vsg_lines
{

    
    const auto lines_vert = R"(
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(push_constant) uniform PushConstants {
    mat4 projection;
    mat4 modelView;
} pc;

layout(location = 0) in vec3 vsg_Vertex;
layout(location = 1) in vec3 vsg_Color;
layout(location = 0) out vec3 color;
layout(location = 1) out vec3 pos;

out gl_PerVertex{
 vec4 gl_Position;
// float gl_PointSize;
};

void main()
{
    vec4 v = pc.projection * pc.modelView * vec4(vsg_Vertex, 1.0);
    gl_Position = v;
    color = vsg_Color.xyz;
    pos = v.xyz/v.w;
}
)";

    const auto lines_frag = R"(
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 color;
layout(location = 1) in vec3 pos;
layout(location = 0) out vec4 outColor;

void main()
{
    vec2 d =1/fwidth(pos.xy);
    float n = max(length(d), 1.0);
    outColor = vec4(color, n);
}
)";

    LinesP::LinesP()
    {

        strip = true;
        bezierMode = false;
        bezierInterpolationPoints = 20;
        node = vsg::Group::create();
    }

    LinesP::~LinesP(void)
    {
    }

    void LinesP::appendData(Vector v, Color c)
    {
        vertices.push_back(vsg::vec3(v.x, v.y, v.z));
        //points->push_back(osg::Vec3(v.x, v.y, v.z));
        colors.push_back(vsg::vec4(c.r, c.g, c.b, c.a));
        
        dirty();
    }

    void LinesP::setData(const std::list<Vector> &p)
    {
        std::list<Vector>::const_iterator it=p.begin();
        vertices.clear();
        colors.clear();
        //points->clear();
        for(;it!=p.end(); ++it)
        {
            vertices.push_back(vsg::vec3(it->x, it->y, it->z));
            colors.push_back(vsg::vec4(1, 1, 1, 1));
            //points->push_back(osg::Vec3(it->x, it->y, it->z));
        }
        dirty();
    }

    void LinesP::setData(const std::list<Vector> &p, const std::list<Color> &colors_)
    {
        std::list<Vector>::const_iterator it=p.begin();
        std::list<Color>::const_iterator it2=colors_.begin();
        vertices.clear();
        colors.clear();

        if(colors_.size() != p.size())
        {
            fprintf(stderr, "Lines::setData(points, colors): size of input lists differs: num points: %lu\t num colors: %lu \t fall back to white as points color!", p.size(), colors_.size());
            setData(p);
            return;
        }
        for(;it!=p.end(); ++it, ++it2)
        {
            vertices.push_back(vsg::vec3(it->x, it->y, it->z));
            colors.push_back(vsg::vec4(it2->r, it2->g, it2->b, it2->a));
        }
        dirty();
    }

    void LinesP::drawStrip(bool strip)
    {
        if(this->strip != strip)
        {
            this->strip = strip;
            dirty();
        }
    }

    void LinesP::setColor(Color c)
    {
        // todo: iterate over colors and replace it
        //dirty();
    }

    void LinesP::setLineWidth(double w)
    {
        //dirty();
    }

    vsg::vec3 LinesP::getBezierPoint(float t)
    {
        return vsg::vec3();
    }

    void LinesP::dirty(void)
    {
        // todo: create and update instance
        if(vertices.size() < 2) return;

        // recreate arrays
        vsg::ref_ptr<vsg::vec3Array> vsgVertices;
        vsg::ref_ptr<vsg::vec4Array> vsgColors;
        size_t numVertices = vertices.size();
        if(!strip)
        {
            vsgVertices = vsg::vec3Array::create(numVertices);
            vsgColors = vsg::vec4Array::create(numVertices);
            std::memcpy(vsgVertices->dataPointer(), vertices.data(), numVertices * 12);
            std::memcpy(vsgColors->dataPointer(), colors.data(), numVertices * 16);
        } else
        {
            numVertices = vertices.size()*2-2;
            vsgVertices = vsg::vec3Array::create(numVertices);
            vsgColors = vsg::vec4Array::create(numVertices);
            std::vector<vsg::vec3> vertices2;
            std::vector<vsg::vec4> colors2;
            vertices2.resize(numVertices);
            colors2.resize(numVertices);

            for(int i=1; i< vertices.size(); ++i)
            {
                vertices2[i*2-2] = vertices[i-1];
                vertices2[i*2-1] = vertices[i];
                colors2[i*2-2] = colors[i-1];
                colors2[i*2-1] = colors[i];
            }
            std::memcpy(vsgVertices->dataPointer(), vertices2.data(), numVertices * 12);
            std::memcpy(vsgColors->dataPointer(), colors2.data(), numVertices * 16);
        }

        // at the moment we always have to recreate the stategroup due the the arrays binding
        // todo: find better solution / check dynmic arrays example
        if(true)
        {

            if(stateGroup)
            {
                auto it = std::find(node->children.begin(), node->children.end(), stateGroup);
                if(it != node->children.end())
                {
                    node->children.erase(it);
                }
            }
            auto options = vsg::Options::create();
            options->fileCache = vsg::getEnv("VSG_FILE_CACHE");
            options->paths = vsg::getEnvPaths("VSG_FILE_PATH");

            auto vertexShader = vsg::ShaderStage::create(VK_SHADER_STAGE_VERTEX_BIT, "main", lines_vert);
            auto fragmentShader = vsg::ShaderStage::create(VK_SHADER_STAGE_FRAGMENT_BIT, "main", lines_frag);

#define VIEW_DESCRIPTOR_SET 0
#define MATERIAL_DESCRIPTOR_SET 1

            auto shaderSet = vsg::ShaderSet::create(vsg::ShaderStages{vertexShader, fragmentShader});

            shaderSet->addAttributeBinding("vsg_Vertex", "", 0, VK_FORMAT_R32G32B32_SFLOAT, vsg::vec3Array::create(1));
            shaderSet->addAttributeBinding("vsg_Color", "", 1, VK_FORMAT_R32G32B32A32_SFLOAT, vsg::vec4Array::create(1), vsg::CoordinateSpace::LINEAR);

            shaderSet->addDescriptorBinding("lightData", "", VIEW_DESCRIPTOR_SET, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, vsg::vec4Array::create(64));

            shaderSet->addPushConstantRange("pc", "", VK_SHADER_STAGE_ALL, 0, 128);
            shaderSet->customDescriptorSetBindings.push_back(vsg::ViewDependentStateBinding::create(VIEW_DESCRIPTOR_SET));

            VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
            colorBlendAttachment.blendEnable = VK_TRUE;
            colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
                VK_COLOR_COMPONENT_G_BIT |
                VK_COLOR_COMPONENT_B_BIT |
                VK_COLOR_COMPONENT_A_BIT;

            colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
            colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
            colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
            colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

            auto colorBlendState = vsg::ColorBlendState::create(vsg::ColorBlendState::ColorBlendAttachments{colorBlendAttachment});
            shaderSet->defaultGraphicsPipelineStates.push_back(colorBlendState);

            auto graphicsPipelineConfig = vsg::GraphicsPipelineConfigurator::create(shaderSet);

            vsg::DataList vertexArrays;
            graphicsPipelineConfig->assignArray(vertexArrays, "vsg_Vertex", VK_VERTEX_INPUT_RATE_VERTEX, vsgVertices);
            graphicsPipelineConfig->assignArray(vertexArrays, "vsg_Color", VK_VERTEX_INPUT_RATE_VERTEX, vsgColors);

            struct SetPipelineStates : public vsg::Visitor
            {

                void apply(vsg::Object& object) override
                    {
                        object.traverse(*this);
                    }

                void apply(vsg::InputAssemblyState& ias) override
                    {
                        //ias.topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
                        ias.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
                    }
                void apply(vsg::RasterizationState& rs) override
                    {
                        rs.lineWidth = 1.0f;
                    }
            } sps;

            graphicsPipelineConfig->accept(sps);

            auto vertexDraw = vsg::VertexDraw::create();
            vertexDraw->assignArrays(vertexArrays);
            vertexDraw->vertexCount = static_cast<uint32_t>(numVertices);
            vertexDraw->instanceCount = 1;
            vertexDraw->firstBinding = graphicsPipelineConfig->baseAttributeBinding;

            graphicsPipelineConfig->init();

            stateGroup = vsg::StateGroup::create();

            graphicsPipelineConfig->copyTo(stateGroup);

            stateGroup->addChild(vertexDraw);
        }
        node->addChild(stateGroup);
    }

    void* LinesP::getVSGNode()
    {
        return (void*)(vsg::Node*)node.get();
    }

    void LinesP::setBezierMode(bool bezier = true)
    {
        bezierMode = bezier;
    }

    void LinesP::setBezierInterpolationPoints(int numPoints)
    {
        bezierInterpolationPoints = numPoints;
    }

} // end of namespace: vsg_lines
