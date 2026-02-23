#include "PointsP.hpp"

#include <cstdio>

/*
 * todo:
 *       - implement node mask
 *       - implement array update instead of rebuilding the whole pipeline on change
 *       - implement option to change color or vertice dynamically
 *         - flag wether the lines are dynamic
 *
 **/

namespace vsg_points
{

    
    const auto points_vert = R"(
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(push_constant) uniform PushConstants {
    mat4 projection;
    mat4 modelView;
} pc;

layout(constant_id = 3) const int viewportDataSize = 1;
layout(set = 0, binding = 1) uniform ViewportData
{
    vec4 view[viewportDataSize];
} vd;

layout(location = 0) in vec3 vsg_Vertex;
layout(location = 1) in vec3 vsg_Color;
layout(location = 0) out vec3 color;
layout(location = 1) out vec4 cpos;
layout(location = 2) out vec4 pos;
layout(location = 3) out float scale;
layout(location = 4) out float depth;

out gl_PerVertex{
 vec4 gl_Position;
 //float gl_PointSize;
};

void main()
{
    vec4 e = pc.modelView * vec4(vsg_Vertex, 1.0);
    vec4 v = pc.projection * e;
    int m = int(floor(mod(gl_VertexIndex, 4)+0.1));
    vec2 offsets[4] = {{1, 1}, {-1, 1}, {-1, -1}, {1, -1}};
    cpos = e;
    float s = 0.015;
    scale = 1.0/s;
    float ratio = vd.view[0][3] / vd.view[0][2];
    e.x += offsets[m].x*s;
    e.y += offsets[m].y*s;
    v = pc.projection * e;
    gl_Position = v;
    pos = e;
    depth = v.z/v.w;
    color = vsg_Color.xyz;
}
)";

    const auto points_frag = R"(
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 color;
layout(location = 1) in vec4 cpos;
layout(location = 2) in vec4 pos;
layout(location = 3) in float scale;
layout(location = 4) in float depth;

layout(location = 0) out vec4 outColor;

void main()
{
   float a = 1-length(pos.xy-cpos.xy)*scale;

   outColor = vec4(color, a);
   float d = depth;
   if(a < 0.25) d = 0;
   gl_FragDepth = d;
}
)";

    PointsP::PointsP() : color(1, 1, 1, 1)
    {
        node = vsg::Group::create();
    }

    PointsP::~PointsP(void)
    {
    }

    void PointsP::appendData(Vector v)
    {
        vertices.push_back(vsg::vec3(v.x, v.y, v.z));
        colors.push_back(vsg::vec4(color.r, color.g, color.b, color.a));
        dirty();
    }

    void PointsP::setData(const std::list<Vector> &p)
    {
        std::list<Vector>::const_iterator it=p.begin();
        vertices.clear();
        colors.clear();
        //points->clear();
        for(;it!=p.end(); ++it)
        {
            vertices.push_back(vsg::vec3(it->x, it->y, it->z));
            colors.push_back(vsg::vec4(color.r, color.g, color.b, color.a));
            //points->push_back(osg::Vec3(it->x, it->y, it->z));
        }
        dirty();
    }

    void PointsP::setColors(const std::vector<Color> &c)
    {
        if(c.size() != vertices.size())
        {
            fprintf(stderr, "Points::setColors(colors): size of input colors differs: num points: %lu\t num colors: %lu \t colors are not applied!", vertices.size(), c.size());
            return;
        }
        std::vector<Color>::const_iterator it=c.begin();
        colors.clear();
        for(;it!=c.end(); ++it)
        {
            colors.push_back(vsg::vec4(it->b, it->g, it->r, it->a));
        }
        dirty();
    }

    void PointsP::setColor(Color c)
    {
        color = c;
        for(auto &it : colors)
        {
            it[0] = c.r;
            it[1] = c.g;
            it[2] = c.b;
            it[3] = c.a;
        }
        dirty();
    }

    void PointsP::setLineWidth(double w)
    {
        pointsize = w;
        //dirty();
    }

    void PointsP::dirty(void)
    {
        // todo: create and update instance
        if(vertices.size() < 1) return;

        // recreate arrays
        vsg::ref_ptr<vsg::vec3Array> vsgVertices;
        vsg::ref_ptr<vsg::vec4Array> vsgColors;
        vsg::ref_ptr<vsg::uintArray> vsgIndices;
        size_t numVertices = vertices.size();
        vsgVertices = vsg::vec3Array::create(numVertices*4);
        vsgColors = vsg::vec4Array::create(numVertices*4);
        vsgIndices = vsg::uintArray::create(numVertices*6);
        for(int i=0; i< vertices.size(); ++i)
        {
            vsgVertices->at(i*4) = vertices[i];
            vsgVertices->at(i*4+1) = vertices[i];
            vsgVertices->at(i*4+2) = vertices[i];
            vsgVertices->at(i*4+3) = vertices[i];
            vsgColors->at(i*4) = colors[i];
            vsgColors->at(i*4+1) = colors[i];
            vsgColors->at(i*4+2) = colors[i];
            vsgColors->at(i*4+3) = colors[i];
            vsgIndices->at(i*6) = i*4;
            vsgIndices->at(i*6+1) = i*4+1;
            vsgIndices->at(i*6+2) = i*4+2;
            vsgIndices->at(i*6+3) = i*4+2;
            vsgIndices->at(i*6+4) = i*4+3;
            vsgIndices->at(i*6+5) = i*4;
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
            auto vertexShader = vsg::ShaderStage::create(VK_SHADER_STAGE_VERTEX_BIT, "main", points_vert);
            auto fragmentShader = vsg::ShaderStage::create(VK_SHADER_STAGE_FRAGMENT_BIT, "main", points_frag);

#define VIEW_DESCRIPTOR_SET 0
#define MATERIAL_DESCRIPTOR_SET 1

            auto shaderSet = vsg::ShaderSet::create(vsg::ShaderStages{vertexShader, fragmentShader});

            shaderSet->addAttributeBinding("vsg_Vertex", "", 0, VK_FORMAT_R32G32B32_SFLOAT, vsg::vec3Array::create(1));
            shaderSet->addAttributeBinding("vsg_Color", "", 1, VK_FORMAT_R32G32B32A32_SFLOAT, vsg::vec4Array::create(1), vsg::CoordinateSpace::LINEAR);

            shaderSet->addDescriptorBinding("material", "", MATERIAL_DESCRIPTOR_SET, 10, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, vsg::PbrMaterialValue::create(), vsg::CoordinateSpace::LINEAR);
            shaderSet->addDescriptorBinding("lightData", "", VIEW_DESCRIPTOR_SET, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, vsg::vec4Array::create(64));
            shaderSet->addDescriptorBinding("viewportData", "", VIEW_DESCRIPTOR_SET, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, vsg::vec4Value::create(0, 0, 1280, 1024));

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

            auto rasterState = vsg::RasterizationState::create();
            rasterState->cullMode = VK_CULL_MODE_NONE;
            shaderSet->defaultGraphicsPipelineStates.push_back(rasterState);

            auto graphicsPipelineConfig = vsg::GraphicsPipelineConfigurator::create(shaderSet);

            vsg::DataList vertexArrays;
            graphicsPipelineConfig->assignArray(vertexArrays, "vsg_Vertex", VK_VERTEX_INPUT_RATE_VERTEX, vsgVertices);
            graphicsPipelineConfig->assignArray(vertexArrays, "vsg_Color", VK_VERTEX_INPUT_RATE_VERTEX, vsgColors);
            graphicsPipelineConfig->enableDescriptor("material");
            graphicsPipelineConfig->enableDescriptor("viewportData");

            struct SetPipelineStates : public vsg::Visitor
            {

                void apply(vsg::Object& object) override
                    {
                        object.traverse(*this);
                    }

                void apply(vsg::InputAssemblyState& ias) override
                    {
                        ias.topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
                        //ias.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
                    }
                void apply(vsg::RasterizationState& rs) override
                    {
                        rs.lineWidth = 2;
                    }
            } sps;

            //graphicsPipelineConfig->accept(sps);

            auto vertexDraw = vsg::VertexIndexDraw::create();
            vertexDraw->assignArrays(vertexArrays);
            vertexDraw->assignIndices(vsgIndices);
            vertexDraw->indexCount = static_cast<uint32_t>(vsgIndices->size());
            //vertexDraw->vertexCount = static_cast<uint32_t>(numVertices);
            vertexDraw->instanceCount = 1;
            vertexDraw->firstBinding = graphicsPipelineConfig->baseAttributeBinding;

            graphicsPipelineConfig->init();

            stateGroup = vsg::StateGroup::create();

            graphicsPipelineConfig->copyTo(stateGroup);

            stateGroup->addChild(vertexDraw);
        }
        node->addChild(stateGroup);
    }

    void* PointsP::getVSGNode()
    {
        return (void*)(vsg::Node*)node.get();
    }

} // end of namespace: vsg_points
