#ifndef __SCENE_HEADER__
#define __SCENE_HEADER__

#include "Partition.h"
#include "../../Common/Shader.h"
#include "../Buffers/VertexArray.h"
#include "../Buffers/ElementBuffer.h"
#include "../Camera.h"

class Scene: public Partition
{
    public:
        Scene();
        void GenerateLevelParitionData(std::vector<float> Vertices,std::vector<unsigned int> Indices,glm::mat4 Translation,glm::mat4 Rotation,glm::mat4 Scale);
        void DrawScene(Shader* ShaderInstance,Camera* CameraInstance,float NearPlane,float FarPlane,float FieldOfView);
        void LoadFromGLTF(Parser* EngineParser,std::string SceneSource,glm::mat4 Translation,glm::mat4 Rotation,glm::mat4 Scale);
        void DestroyScene();
        std::vector<float>          Vertices;
        std::vector<unsigned int>   Indices;
    private:
        glm::mat4                   TranslationMatrix;
        glm::mat4                   RotationMatrix;
        glm::mat4                   ScaleMatrix;
        VertexArrayObject           SceneVertexArray;
};     

#endif