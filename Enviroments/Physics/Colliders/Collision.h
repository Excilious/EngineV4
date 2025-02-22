#ifndef __COLLISION_HEADER__
#define __COLLISION_HEADER__

#include <vector>
#include "../../Rendering/Buffers/VertexBuffer.h"
#include "../Dependancies/glad/glad.h"
#include "../Dependancies/GLFW/glfw3.h"
#include "../Dependancies/glm/glm.hpp"

typedef struct 
{
    glm::vec3 BoundsMinimum;
    glm::vec3 BoundsMaximum;
} InstanceBounds;


class Collisions
{
    public:
        Collisions();
        int CreatePredefinedBoundsUsingMatrix(std::vector<float> Vertices,std::vector<unsigned int> Indices,glm::mat4 Matrix,float Stride);
        int CreatePredefinedBounds(std::vector<float> Vertices,std::vector<unsigned int> Indices,float Stride);

        bool IsVectorInTriangle(glm::vec3 Point,glm::vec3 Vertex0,glm::vec3 Vertex1,glm::vec3 Vertex2);
        bool IntersectPointWithTriangle(glm::vec3 Point,std::vector<float> Vertices,std::vector<unsigned int> Indices,glm::mat4 ModelMatrix,float Threshold,int Stride);
        bool IntersectPointWithBounds(glm::vec3 Point,int CachedIndex);
        void CleanUp();
    private:
        const float Epslion = 1e-6f;
        glm::vec3 GetVertexPositions(std::vector<float> Vertices,size_t Index,int Stride);
        std::vector<InstanceBounds*>    CachedElementBounds;
};

#endif