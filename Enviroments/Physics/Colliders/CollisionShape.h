#ifndef __COLLISION_SHAPE_HEADER__
#define __COLLISION_SHAPE_HEADER__

#define COLLISION_SHAPE_DEBUG_RENDER

#include <vector>
#include "../Dependancies/glm/gtc/type_ptr.hpp"
#include "../Dependancies/glad/glad.h"
#include "../Dependancies/GLFW/glfw3.h"
#include "../Dependancies/glm/glm.hpp"

class CollisionShape
{
    public:
        CollisionShape();
        void GenerateCollisionShapeFromVertex(std::vector<float> Vertices,std::vector<unsigned int> Indices,glm::mat4 ModelMatrix,unsigned int Stride);

        void RenderPhysicsCollisionShape(unsigned int ShaderProgram,glm::mat4 CameraMatrix);
        void RenderGeneratePhysicsCollisionShape();
    private:
        glm::vec3 GetVertexPositions(std::vector<float> Vertices,size_t Index,int Stride);

        std::vector<glm::vec3>      Vertices;
        std::vector<unsigned int>   Indices;
        unsigned int                PhysicsVertexArray;
        unsigned int                PhysicsElementBuffer;
        unsigned int                PhysicsVertexBuffer;
};

#endif