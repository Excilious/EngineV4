#include "Scene.h"

Scene::Scene()
{}

void Scene::LoadFromGLTF(Parser* EngineParser,std::string SceneSource,glm::mat4 Translation,glm::mat4 Rotation,glm::mat4 Scale)
{
    this->LoadPartitionUsingGLTF(EngineParser,SceneSource);
    if (this->PartitionVertices.size() != 0 && this->PartitionIndices.size() != 0)
    {
        this->TranslationMatrix     = Translation;
        this->RotationMatrix        = Rotation;
        this->ScaleMatrix           = Scale;
        this->Vertices              = this->FloatPositionVectors;
        this->Indices               = PartitionIndices;

        this->SceneVertexArray.Bind();
        VertexBufferObject* SceneVertexBuffer       = new VertexBufferObject(this->PartitionVertices);
        ElementBufferObject* SceneElementBuffer     = new ElementBufferObject(this->PartitionIndices);

        this->SceneVertexArray.LinkAttribute(*SceneVertexBuffer,0,3,GL_FLOAT,sizeof(SceneVertex),(void*)0);
        this->SceneVertexArray.LinkAttribute(*SceneVertexBuffer,1,3,GL_FLOAT,sizeof(SceneVertex),(void*)(3*sizeof(float)));

        this->SceneVertexArray.Unbind();
        SceneVertexBuffer->Unbind();
        SceneElementBuffer->Unbind();
        if (SceneVertexBuffer)  {delete SceneVertexBuffer;}
        if (SceneElementBuffer) {delete SceneElementBuffer;}
    }
    else
    {SysPrint("Failed to load "+SceneSource+" level parition.",2);}
}

void Scene::GenerateLevelParitionData(std::vector<float> Vertices,std::vector<unsigned int> Indices,glm::mat4 Translation,glm::mat4 Rotation,glm::mat4 Scale)
{
    this->Vertices              = Vertices;
    this->Indices               = Indices;
    this->TranslationMatrix     = Translation;
    this->RotationMatrix        = Rotation;
    this->ScaleMatrix           = Scale;

    this->SceneVertexArray.Bind();
    VertexBufferObject* SceneVertexBuffer   = new VertexBufferObject(this->Vertices);
    ElementBufferObject* SceneElementBuffer = new ElementBufferObject(this->Indices);
  
    this->SceneVertexArray.LinkAttribute(*SceneVertexBuffer,0,3,GL_FLOAT,6*sizeof(float),(void*)0);
    this->SceneVertexArray.LinkAttribute(*SceneVertexBuffer,1,3,GL_FLOAT,6*sizeof(float),(void*)(3*sizeof(float)));

    this->SceneVertexArray.Unbind();
    SceneVertexBuffer->Unbind();
    SceneElementBuffer->Unbind();
    if (SceneVertexBuffer)  {delete SceneVertexBuffer;}
    if (SceneElementBuffer) {delete SceneElementBuffer;}
}

void Scene::DrawScene(Shader* ShaderInstance,Camera* CameraInstance,float NearPlane,float FarPlane,float FieldOfView)
{
    ShaderInstance->ActivateShader();
    this->SceneVertexArray.Bind();
    glUniform3f(glGetUniformLocation(ShaderInstance->Program,"CameraPosition"),CameraInstance->Position.x,CameraInstance->Position.y,CameraInstance->Position.z);
    CameraInstance->UpdateMatrix(FieldOfView,NearPlane,FarPlane);
    CameraInstance->SetMatrix(ShaderInstance,"CameraMatrix");

    glUniformMatrix4fv(glGetUniformLocation(ShaderInstance->Program,"Translation"),1,GL_FALSE,glm::value_ptr(this->TranslationMatrix));
    glUniformMatrix4fv(glGetUniformLocation(ShaderInstance->Program,"Rotation"),1,GL_FALSE,glm::value_ptr(this->RotationMatrix));
    glUniformMatrix4fv(glGetUniformLocation(ShaderInstance->Program,"Scale"),1,GL_FALSE,glm::value_ptr(this->ScaleMatrix));
    glDrawElements(GL_TRIANGLES,this->Indices.size(),GL_UNSIGNED_INT,0);
    this->SceneVertexArray.Unbind();
}

void Scene::DestroyScene()
{this->SceneVertexArray.Delete();}