#ifndef __MODEL_HEADER__
#define __MODEL_HEADER__

#include "Mesh.h"
#include "../Common/Parser.h"
#include "../Dependancies/jsonimport/json.h"

class Model : public Mesh
{
    public:
        Model(std::string ModelName,std::string ModelFile,Parser* NewParser,glm::vec3 Position,glm::quat Rotation,glm::vec3 Scale);
        void DrawModel(Shader* NewShader,Camera* NewCamera,float NearPlane,float FarPlane,float FieldOfView);
        void LoadModel(unsigned int MeshIndex);
        void TraverseModel(unsigned int NextNode);
        void DestroyModel();
        std::vector<float> GetFloats(nlohmann::json Accessor);
    private:
        std::vector<Texture*>       GetTexture();
        std::vector<glm::vec2>      GroupVector2(std::vector<float> FloatVector);
        std::vector<glm::vec3>      GroupVector3(std::vector<float> FloatVector);
        std::vector<glm::vec4>      GroupVector4(std::vector<float> FloatVector);
        std::vector<GLuint>         GetIndices(nlohmann::json Accessor);
        std::vector<Vertex>         AssembleVertices(std::vector<glm::vec3> Position,std::vector<glm::vec3> Normal,std::vector<glm::vec2> TextureUV);
        std::vector<unsigned char>  NewData;

        std::vector<glm::vec3>      Positions;
        std::vector<glm::vec3>      Normals;
        std::vector<glm::vec2>      TextureUV;
        std::vector<std::string>    LoadedTextureName;
        std::vector<Texture*>       LoadedTexture;

        nlohmann::json              Json;
        std::string                 Contents;
        std::string                 ModelName;
        glm::vec3                   TranslationModel;
        glm::quat                   RotationModel;
        glm::vec3                   ScaleModel;
};

#endif