#ifndef INCUHENGINE_GAMETRANSFORM_HPP
#define INCUHENGINE_GAMETRANSFORM_HPP
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>


namespace Incuh {
    class Transform {
    public:
        Transform(float x = 0.0f, float y = 0.0f, float z = 0.0f, float q = 0.0f, float qw = 0.0f, float qy = 0.0f, float qz = 0.0f, float sx = 1.0f, float sy = 1.0f, float sz = 1.0f);
        virtual ~Transform();

        float* glmToFloat(glm::vec3 val);
        float* glmToFloat(glm::quat val);

        glm::vec3 getPosition();
        glm::quat getRotation();
        glm::vec3 getScale();

        void setX( float x );
        void setY( float y );
        void setZ( float z );

        void setRotationW( float w );
        void setRotationX( float x );
        void setRotationY( float y );
        void setRotationZ( float z );

        void setScaleX( float x );
        void setScaleY( float y );
        void setScaleZ( float z );

        void setPosition( float x, float y, float z );
        void setPosition( glm::vec3 newPos );

        void setRotation( float q, float x, float y, float z );
        void setRotation( glm::quat newPos );

        void setScale( float x, float y, float z );
        void setScale( glm::vec3 newPos );


    private:
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;
        std::vector<float*> allocatedFloats;
    };
}

#endif //INCUHENGINE_GAMETRANSFORM_HPP
