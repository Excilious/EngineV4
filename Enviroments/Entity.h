#ifndef __ENTITY_HEADER__
#define __ENTITY_HEADER__

#include "Rendering/Model.h"

typedef enum
{
    ENT_STATIC,
    ENT_DYNAMIC
} EntityType;

class Entity : public Model
{
    public:
        Entity(std::string EntityName,std::string ModelFile,Parser* NewParser,glm::vec3 Position,glm::quat Rotation,glm::vec3 Scale);
        void CreateMeshShadowMap();
    private:
        GLuint  ShadowMapFrameBuffer;
        GLuint  ShadowMap;
        GLuint  ShadowMapWidth,ShadowMapHeigh = 2048;
        float   ClampColour[4] = {1.0f,1.0f,1.0f,1.0f};
};

#endif