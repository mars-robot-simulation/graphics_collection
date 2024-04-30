#pragma once

#include <osg/Drawable>
#include <cstdio>

namespace osg_terrain
{

    class MultiResHeightMapRenderer;
    class VertexBufferTerrain : public osg::Drawable
    {

    public:
        VertexBufferTerrain(int width, int height, double scaleZ, int resolution,
                            int depth);  
        //VertexBufferTerrain(const interfaces::terrainStruct *ts);

        VertexBufferTerrain(const VertexBufferTerrain &pg,
                            const osg::CopyOp &copyop=osg::CopyOp::SHALLOW_COPY)
            {
                fprintf(stderr, "error: not implemented yet!!");
            }

        virtual ~VertexBufferTerrain();

        virtual osg::Object* cloneType() const
            {
                fprintf(stderr, "error: not implemented yet!!");
                return new VertexBufferTerrain(0, 0, 0, 0, 0);
            }

        virtual osg::Object* clone(const osg::CopyOp& copyop) const
            {
                fprintf(stderr, "error: not implemented yet!!");
                return new VertexBufferTerrain (*this, copyop);
            }

        virtual void drawImplementation(osg::RenderInfo& renderInfo) const;
        void collideSphere(double xPos, double yPos, double zPos, double radius);
#if (OPENSCENEGRAPH_MAJOR_VERSION < 3 || ( OPENSCENEGRAPH_MAJOR_VERSION == 3 && OPENSCENEGRAPH_MINOR_VERSION < 4))
        osg::BoundingBox computeBound() const;
#elif (OPENSCENEGRAPH_MAJOR_VERSION > 3 || (OPENSCENEGRAPH_MAJOR_VERSION == 3 && OPENSCENEGRAPH_MINOR_VERSION >= 4))
        osg::BoundingSphere computeBound() const;
#else
#error Unknown OSG Version
#endif
        void setSelected(bool val);
        void setCameraPosition(double x, double y);

    private:
        MultiResHeightMapRenderer *mrhmr;
        double width, height, scale;

    }; // end of class VertexBufferTerrain

} // end of namespace osg_terrain
