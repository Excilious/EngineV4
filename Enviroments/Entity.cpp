#include "Entity.h"

Entity::Entity(std::string EntityName,std::string ModelFile,Parser* NewParser,glm::vec3 Position,glm::quat Rotation,glm::vec3 Scale) : Model(EntityName,ModelFile,NewParser,Position,Rotation,Scale)
{}

void Entity::CreateMeshShadowMap()
{
    // glGenTextures(1,&this->ShadowMap);
    // glBindTexture(GL_TEXTURE_2D,this->ShadowMap);
    // glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,this->ShadowMapWidth,this->ShadowMapHeigh,0,GL_DEPTH_COMPONENT,GL_FLOAT,GL_FALSE);

    // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
    // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);

    // glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,this->ClampColour);
    // glBindFramebuffer(GL_FRAMEBUFFER,this->ShadowMapFrameBuffer);
    // glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,this->ShadowMap,0);
    // glDrawBuffer(GL_NONE);
    // glReadBuffer(GL_NONE);
    // glBindFramebuffer(GL_FRAMEBUFFER,0);
}