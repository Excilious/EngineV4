#ifndef __MESH_HEADER__
#define __MESH_HEADER__

#include <string>
#include "Buffers/VertexArray.h"
#include "Buffers/ElementBuffer.h"
#include "Camera.h"
#include "Texture.h"

class Mesh
{
    public:
        Mesh(glm::vec3 Position,glm::quat Rotation,glm::vec3 Scale);
        void Generate(std::vector<Vertex>& Vertices,std::vector<GLuint>& Indices,std::vector<Texture*>& Textures);
        void Draw(Shader* ShaderInstance,Camera* CameraInstance,float NearPlane,float FarPlane,float FieldOfView);
        void Destroy();
        VertexArrayObject       VertexArray;
    private:
        std::vector <Vertex>    Vertices;
        std::vector <GLuint>    Indices;
        std::vector <Texture*>  Textures;
        glm::vec3               TranslationMesh;
        glm::quat               RotationMesh;
        glm::vec3               ScaleMesh;
        glm::mat4               NewTranslation; 
        glm::mat4               NewRotation;
        glm::mat4               NewScale;
};

#endif