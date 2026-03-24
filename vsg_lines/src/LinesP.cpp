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

layout(constant_id = 3) const int viewportDataSize = 1;
layout(set = 0, binding = 1) uniform ViewportData
{
    vec4 view[viewportDataSize];
} vd;

layout(set = 0, binding = 5) uniform WorldTransform{
    mat4 projectionInverse;
    mat4 viewInverse;
} wt;

layout(location = 0) in vec3 vsg_Vertex;
layout(location = 1) in vec3 vsg_Dir;
layout(location = 2) in vec3 vsg_Color;
layout(location = 0) out vec3 color;
layout(location = 1) out vec4 pos;
layout(location = 2) out float distanceToLine;

out gl_PerVertex{
 vec4 gl_Position;
// float gl_PointSize;
};

void main()
{
    mat4 modelView = inverse(wt.viewInverse);
    mat4 viewInverse = inverse(pc.modelView);
    mat4 projectionInverse = inverse(pc.projection);

    vec4 e = modelView * vec4(vsg_Vertex, 1.0);
    vec4 v = pc.projection * e;
    vec4 d = pc.projection * modelView * vec4(vsg_Vertex+vsg_Dir, 1.0);
    vec2 d2 = vec2(d.xy/d.w-v.xy/v.w);
    float ratio = vd.view[0][3] / vd.view[0][2];
    vec4 offset = projectionInverse * vec4(-d2.y*ratio, d2.x, 0, 1);

    int m = int(floor(mod(gl_VertexIndex, 4)+0.1));
    float direction[4] = {1, -1, 1, -1};

    vec3 t = normalize(vec3(offset.xy, 0));

    // float fade = (-e.z-1)*0.02;
    // fade = max(min(fade, 1.0), 0.0);
    // fade = 0;
    float expand = 0.04; //-fade*0.04;
    v = vec4(e.xy + t.xy*expand*direction[m], e.z, e.w);
    v = pc.projection*v;
    color = vsg_Color.xyz;

    gl_Position = v;

    pos = v;
    distanceToLine = direction[m];  //length(d.xy);
}
)";

    const auto lines_frag = R"(
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 color;
layout(location = 1) in vec4 pos;
layout(location = 2) in float distanceToLine;

layout(location = 0) out vec4 outColor;

void main()
{
    float d = abs(distanceToLine)*10;
    float derivative = fwidth(d);
    float a =  1-d;
    a = min(max(sqrt(a), 0), 1);
    outColor = vec4(color, a);
    float depth = pos.z/pos.w;
    if(a < 0.25) depth = 0;
    gl_FragDepth = depth;
}
)";

    struct WorldTransformUniform
    {
        vsg::mat4 projInverse;
        vsg::mat4 viewInverse;
    };
    using WorldTransformUniformValue = vsg::Value<WorldTransformUniform>;

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
        bool recreate = false;
        if(!strip)
        {
            // each line is represented by a quad, so we have to
            // duplicate the vertices
            if(numVertices != vertices.size())
            {
                numVertices = vertices.size();
                recreate = true;
                vsgVertices = vsg::vec3Array::create(numVertices*2);
                // the normals encode the direction vector of the line
                vsgNormals = vsg::vec3Array::create(numVertices*2);
                vsgColors = vsg::vec4Array::create(numVertices*2);
                vsgIndices = vsg::uintArray::create(numVertices*3);
                vsgVertices->properties.dataVariance = vsg::DYNAMIC_DATA;
                vsgNormals->properties.dataVariance = vsg::DYNAMIC_DATA;
                vsgColors->properties.dataVariance = vsg::DYNAMIC_DATA;
            }
            vsg::vec3 direction;
            for(int i=0; i< vertices.size()/2; ++i)
            {
                direction = vertices[i*2+1] - vertices[i*2];
                vsgVertices->at(i*4) = vertices[i*2];
                vsgVertices->at(i*4+1) = vertices[i*2];
                vsgVertices->at(i*4+2) = vertices[i*2+1];
                vsgVertices->at(i*4+3) = vertices[i*2+1];
                vsgNormals->at(i*4) = direction;
                vsgNormals->at(i*4+1) = direction;
                vsgNormals->at(i*4+2) = direction;
                vsgNormals->at(i*4+3) = direction;
                vsgColors->at(i*4) = colors[i*2];
                vsgColors->at(i*4+1) = colors[i*2];
                vsgColors->at(i*4+2) = colors[i*2+1];
                vsgColors->at(i*4+3) = colors[i*2+1];
                vsgIndices->at(i*6) = i*4;
                vsgIndices->at(i*6+1) = i*4+3;
                vsgIndices->at(i*6+2) = i*4+1;
                vsgIndices->at(i*6+3) = i*4+2;
                vsgIndices->at(i*6+4) = i*4+3;
                vsgIndices->at(i*6+5) = i*4;
            }
        } else
        {
            if(numVertices != vertices.size()*2-2)
            {
                numVertices = vertices.size()*2-2;
                recreate = true;
                vsgVertices = vsg::vec3Array::create(numVertices*2);
                vsgNormals = vsg::vec3Array::create(numVertices*2);
                vsgColors = vsg::vec4Array::create(numVertices*2);
                vsgIndices = vsg::uintArray::create(numVertices*3);
                vsgVertices->properties.dataVariance = vsg::DYNAMIC_DATA;
                vsgNormals->properties.dataVariance = vsg::DYNAMIC_DATA;
                vsgColors->properties.dataVariance = vsg::DYNAMIC_DATA;
            }
            vsg::vec3 direction;
            for(int i=1; i< vertices.size(); ++i)
            {
                direction = vertices[i] - vertices[i-1];
                //direction = vsg::normalize(direction);
                vsgVertices->at(i*4-4) = vertices[i-1];
                vsgVertices->at(i*4-3) = vertices[i-1];
                vsgVertices->at(i*4-2) = vertices[i];
                vsgVertices->at(i*4-1) = vertices[i];
                vsgNormals->at(i*4-4) = direction;
                vsgNormals->at(i*4-3) = direction;
                vsgNormals->at(i*4-2) = direction;
                vsgNormals->at(i*4-1) = direction;
                vsgColors->at(i*4-4) = colors[i-1];
                vsgColors->at(i*4-3) = colors[i-1];
                vsgColors->at(i*4-2) = colors[i];
                vsgColors->at(i*4-1) = colors[i];
                vsgIndices->at(i*6-6) = i*4-4;
                vsgIndices->at(i*6-5) = i*4-1;
                vsgIndices->at(i*6-4) = i*4-3;
                vsgIndices->at(i*6-3) = i*4-2;
                vsgIndices->at(i*6-2) = i*4-1;
                vsgIndices->at(i*6-1) = i*4-4;
            }
        }

        // at the moment we always have to recreate the stategroup due the the arrays binding
        // todo: find better solution / check dynmic arrays example
        if(!recreate)
        {
            vsgVertices->dirty();
            vsgNormals->dirty();
            vsgColors->dirty();
        } else
        {

            node->children.clear();

            auto options = vsg::Options::create();
            options->fileCache = vsg::getEnv("VSG_FILE_CACHE");
            options->paths = vsg::getEnvPaths("VSG_FILE_PATH");

            auto vertexShader = vsg::ShaderStage::create(VK_SHADER_STAGE_VERTEX_BIT, "main", lines_vert);
            auto fragmentShader = vsg::ShaderStage::create(VK_SHADER_STAGE_FRAGMENT_BIT, "main", lines_frag);

#define VIEW_DESCRIPTOR_SET 0
#define MATERIAL_DESCRIPTOR_SET 1

            auto shaderSet = vsg::ShaderSet::create(vsg::ShaderStages{vertexShader, fragmentShader});

            shaderSet->addAttributeBinding("vsg_Vertex", "", 0, VK_FORMAT_R32G32B32_SFLOAT, vsg::vec3Array::create(1));
            shaderSet->addAttributeBinding("vsg_Dir", "", 1, VK_FORMAT_R32G32B32_SFLOAT, vsg::vec3Array::create(1));
            shaderSet->addAttributeBinding("vsg_Color", "", 2, VK_FORMAT_R32G32B32A32_SFLOAT, vsg::vec4Array::create(1), vsg::CoordinateSpace::LINEAR);

            shaderSet->addDescriptorBinding("material", "", MATERIAL_DESCRIPTOR_SET, 10, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, vsg::PbrMaterialValue::create(), vsg::CoordinateSpace::LINEAR);
            shaderSet->addDescriptorBinding("lightData", "", VIEW_DESCRIPTOR_SET, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, vsg::vec4Array::create(64));
            shaderSet->addDescriptorBinding("viewportData", "", VIEW_DESCRIPTOR_SET, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, vsg::vec4Value::create(0, 0, 1280, 1024));
            shaderSet->addDescriptorBinding("worldTransform", "", VIEW_DESCRIPTOR_SET, 5, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, WorldTransformUniformValue::create());

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
            graphicsPipelineConfig->assignArray(vertexArrays, "vsg_Dir", VK_VERTEX_INPUT_RATE_VERTEX, vsgNormals);
            graphicsPipelineConfig->assignArray(vertexArrays, "vsg_Color", VK_VERTEX_INPUT_RATE_VERTEX, vsgColors);
            graphicsPipelineConfig->enableDescriptor("material");
            graphicsPipelineConfig->enableDescriptor("viewportData");
            graphicsPipelineConfig->enableDescriptor("worldTransform");

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
            node->addChild(stateGroup);
        }
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
