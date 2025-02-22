#include "CollisionShape.h"

CollisionShape::CollisionShape()
{}

glm::vec3 CollisionShape::GetVertexPositions(std::vector<float> Vertices,size_t Index,int Stride)
{return glm::vec3(Vertices[(Index*Stride)+2],Vertices[(Index*Stride)+1],Vertices[(Index*Stride)+0]);}

void CollisionShape::GenerateCollisionShapeFromVertex(std::vector<float> Vertices,std::vector<unsigned int> Indices,glm::mat4 ModelMatrix,unsigned int Stride)
{
    std::vector<glm::vec3> VertexPositions;
    for (size_t i = 0; i < Indices.size(); i += 3)
    {std::swap(Indices[i + 1], Indices[i + 2]);}
    for (int Index = 0; Index < Indices.size(); Index++)
    {  
        glm::vec3 Vertex = glm::vec3(ModelMatrix*glm::vec4(this->GetVertexPositions(Vertices,Index,Stride),1.0f));
        VertexPositions.push_back(Vertex);
    }

    this->Vertices  = VertexPositions;
    this->Indices   = Indices;
}

#ifdef COLLISION_SHAPE_DEBUG_RENDER

    void CollisionShape::RenderGeneratePhysicsCollisionShape()
    {
        if (this->Vertices.size() != 0 && this->Indices.size() != 0)
        {
            glGenVertexArrays(1,&PhysicsVertexArray);
            glGenBuffers(1,&PhysicsVertexBuffer);
            glGenBuffers(1,&PhysicsElementBuffer);

            glBindVertexArray(PhysicsVertexArray);
            glBindBuffer(GL_ARRAY_BUFFER,PhysicsVertexBuffer);
            glBufferData(GL_ARRAY_BUFFER,this->Vertices.size()*sizeof(glm::vec3),this->Vertices.data(),GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,PhysicsElementBuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,this->Indices.size()*sizeof(unsigned int),this->Indices.data(),GL_STATIC_DRAW);

            glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(glm::vec3),(void*)0);
            glEnableVertexAttribArray(0);

            glBindVertexArray(0);
        }
    }

    void CollisionShape::RenderPhysicsCollisionShape(unsigned int ShaderProgram,glm::mat4 CameraMatrix)
    {
        glUseProgram(ShaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(ShaderProgram,"CameraMatrix"),1,GL_FALSE,glm::value_ptr(CameraMatrix));

        glBindVertexArray(PhysicsVertexArray);
        glDrawElements(GL_TRIANGLES,this->Indices.size(),GL_UNSIGNED_INT,0);
    }

#endif