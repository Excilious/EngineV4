#ifndef __INSTANCE_HEADER__
#define __INSTANCE_HEADER__

#include <vector>
#include "../Common/Parser.h"
#include "../Rendering/Camera.h"
#include "../Entity.h"

typedef struct
{
    Model       NewModel;
    Shader      ModelShader;
} ModelNode;

typedef struct
{
    Entity          *NewEntityInstance;
    EntityType       NewEntityType;
    Shader           ShaderEntity;
} EntityNode;

class Instance
{
    public:
        Instance();
        void GenerateCharacterEntity();
        void GenerateLightEntity();
        void GenerateStaticEntity(Parser *EntityParser,std::string EntityName,std::string ShaderVert,std::string ShaderFrag,glm::vec3 Position,glm::vec3 Scale,glm::quat Rotation);
        void RenderEntities(Camera *EngineCamera,float Near,float Far,float FieldOfView);
        void DestroyAll();
    private:
        std::vector<EntityNode>     EntityList;
        std::vector<ModelNode*>     ModelList;
};

#endif