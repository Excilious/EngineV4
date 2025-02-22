#include "Instance.h"

Instance::Instance()
{}

void Instance::GenerateStaticEntity(Parser *EntityParser,std::string EntityName,std::string ShaderVert,std::string ShaderFrag,glm::vec3 Position,glm::vec3 Scale,glm::quat Rotation)
{
    EntityNode Node;
    Entity* EntityModel = new Entity("Model",EntityName,EntityParser,Position,Rotation,Scale);
    Shader  EntityShader;

    EntityShader.CreateNewShader(ShaderVert.c_str(),ShaderFrag.c_str(),true);
    Node.NewEntityInstance = EntityModel;
    Node.ShaderEntity      = EntityShader;
    Node.NewEntityType     = ENT_STATIC;
    this->EntityList.push_back(Node);
}

void Instance::RenderEntities(Camera *EngineCamera,float Near,float Far,float FieldOfView)
{
    for (int Index = 0; Index < this->EntityList.size(); Index++)
    {
        this->EntityList[Index].NewEntityInstance->DrawModel(&this->EntityList[Index].ShaderEntity,EngineCamera,Near,Far,FieldOfView);
        this->EntityList[Index].ShaderEntity.ActivateShader();
    }
}

void Instance::DestroyAll()
{
    for (int Index = 0; Index < this->EntityList.size(); Index++)
    {
        this->EntityList[Index].NewEntityInstance->Destroy();
        if (this->EntityList[Index].NewEntityInstance) {delete this->EntityList[Index].NewEntityInstance;}
    }
}