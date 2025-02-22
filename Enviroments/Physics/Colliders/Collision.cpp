#include "Collision.h"

Collisions::Collisions()
{}

glm::vec3 Collisions::GetVertexPositions(std::vector<float> Vertices,size_t Index,int Stride)
{return glm::vec3(Vertices[(Index*Stride)+2],Vertices[(Index*Stride)+1],Vertices[(Index*Stride)+0]);}

int Collisions::CreatePredefinedBoundsUsingMatrix(std::vector<float> Vertices,std::vector<unsigned int> Indices,glm::mat4 Matrix,float Stride)
{
    this->CreatePredefinedBounds(Vertices,Indices,Stride);
    this->CachedElementBounds.back()->BoundsMaximum = glm::vec3(Matrix*glm::vec4(this->CachedElementBounds.back()->BoundsMaximum,1.0f));
    this->CachedElementBounds.back()->BoundsMinimum = glm::vec3(Matrix*glm::vec4(this->CachedElementBounds.back()->BoundsMinimum,1.0f));
    return this->CachedElementBounds.size();
}

int Collisions::CreatePredefinedBounds(std::vector<float> Vertices,std::vector<unsigned int> Indices,float Stride)
{
    glm::vec3 Minimum = glm::vec3(Vertices[(Indices[0]*Stride)+0],Vertices[(Indices[0]*Stride)+1],Vertices[(Indices[0]*Stride)+2]);
    glm::vec3 Maximum = glm::vec3(Vertices[(Indices[0]*Stride)+0],Vertices[(Indices[0]*Stride)+1],Vertices[(Indices[0]*Stride)+2]);

    for (int Index = 0; Index < Indices.size(); Index++)
    {
        if (Vertices[(Indices[Index]*Stride)+0] < Minimum.x) {Minimum.x = Vertices[(Indices[Index]*Stride)+0];}
        if (Vertices[(Indices[Index]*Stride)+0] > Maximum.x) {Maximum.x = Vertices[(Indices[Index]*Stride)+0];}
        if (Vertices[(Indices[Index]*Stride)+1] < Minimum.y) {Minimum.y = Vertices[(Indices[Index]*Stride)+1];}
        if (Vertices[(Indices[Index]*Stride)+1] > Maximum.y) {Maximum.y = Vertices[(Indices[Index]*Stride)+1];}
        if (Vertices[(Indices[Index]*Stride)+2] < Minimum.z) {Minimum.z = Vertices[(Indices[Index]*Stride)+2];}
        if (Vertices[(Indices[Index]*Stride)+2] > Maximum.z) {Maximum.z = Vertices[(Indices[Index]*Stride)+2];}
    }
    
    InstanceBounds* NewInstanceBounds   = new InstanceBounds();
    NewInstanceBounds->BoundsMaximum    = Maximum;
    NewInstanceBounds->BoundsMinimum    = Minimum;
    this->CachedElementBounds.push_back(NewInstanceBounds);
    return this->CachedElementBounds.size();
}

bool Collisions::IntersectPointWithBounds(glm::vec3 Point,int CachedIndex)
{
    if (Point.x > this->CachedElementBounds[CachedIndex]->BoundsMaximum.x || Point.x < this->CachedElementBounds[CachedIndex]->BoundsMinimum.x) {return false;}
    if (Point.y > this->CachedElementBounds[CachedIndex]->BoundsMaximum.y || Point.y < this->CachedElementBounds[CachedIndex]->BoundsMinimum.y) {return false;}
    if (Point.z > this->CachedElementBounds[CachedIndex]->BoundsMaximum.z || Point.z < this->CachedElementBounds[CachedIndex]->BoundsMinimum.z) {return false;}
    return true;
}

bool Collisions::IsVectorInTriangle(glm::vec3 Point, glm::vec3 Vertex0, glm::vec3 Vertex1, glm::vec3 Vertex2)
{
    glm::vec3 VectorCompute0    = Vertex1-Vertex0;
    glm::vec3 VectorCompute1    = Vertex2-Vertex0;
    glm::vec3 VectorCompute2    = Point-Vertex0;

    float Dot00                 = glm::dot(VectorCompute0,VectorCompute0);
    float Dot01                 = glm::dot(VectorCompute0,VectorCompute1);
    float Dot02                 = glm::dot(VectorCompute0,VectorCompute2);
    float Dot11                 = glm::dot(VectorCompute1,VectorCompute1);
    float Dot12                 = glm::dot(VectorCompute1,VectorCompute2);

    float Denominator           = Dot00*Dot11-Dot01*Dot01;
    if (Denominator == 0) {return false;}
    
    float InverseDenom          = 1.0f/Denominator;

    float U                     = (Dot11*Dot02-Dot01*Dot12) * InverseDenom;
    float V                     = (Dot00*Dot12-Dot01*Dot02) * InverseDenom;

    return (U >= 0) && (V >= 0) && (U+V <= 1);
}

bool Collisions::IntersectPointWithTriangle(glm::vec3 Point,std::vector<float> Vertices,std::vector<unsigned int> Indices,glm::mat4 ModelMatrix,float Threshold,int Stride)
{
    for (size_t i = 0; i < Indices.size(); i += 3)
    {std::swap(Indices[i + 1], Indices[i + 2]);}
    for (int Index = 0; Index < Indices.size(); Index += 3)
    {
        glm::vec3 Vertex0 = glm::vec3(ModelMatrix*glm::vec4(this->GetVertexPositions(Vertices,Indices[Index+0],Stride),1.0f));
        glm::vec3 Vertex1 = glm::vec3(ModelMatrix*glm::vec4(this->GetVertexPositions(Vertices,Indices[Index+1],Stride),1.0f));
        glm::vec3 Vertex2 = glm::vec3(ModelMatrix*glm::vec4(this->GetVertexPositions(Vertices,Indices[Index+2],Stride),1.0f));

        // glm::vec3 VertexA = Vertex0-Point;
        // glm::vec3 VertexB = Vertex1-Point;
        // glm::vec3 VertexC = Vertex2-Point;

        // glm::vec3 NormalA = glm::cross(VertexB,VertexC);
        // glm::vec3 NormalB = glm::cross(VertexC,VertexA);
        // glm::vec3 NormalC = glm::cross(VertexA,VertexB);

        // if (glm::dot(NormalA,NormalB) < 0.0f)
        // {return false;}
        // else if (glm::dot(NormalA,NormalC) < 0.0f)
        // {return false;}

        glm::vec3 Edge1     = Vertex1-Vertex0;
        glm::vec3 Edge2     = Vertex2-Vertex0;
        glm::vec3 Normal    = glm::normalize(glm::cross(Edge1, Edge2));

        if (glm::abs(glm::dot(Normal,Point-Vertex0)) < Threshold+Epslion)
        {
            if (this->IsVectorInTriangle(Point,Vertex0,Vertex1,Vertex2))
            {return true;}
        }
    }
    return false;
}

// bool Collisions::IntersectPointWithTriangle(glm::vec3 Point, std::vector<float> Vertices, std::vector<unsigned int> Indices, glm::mat4 ModelMatrix, float Threshold, int Stride)
// {
//     // Ensure the triangle winding order is correct
//     for (size_t i = 0; i < Indices.size(); i += 3)
//     {
//         std::swap(Indices[i + 1], Indices[i + 2]); 
//     }

//     // Iterate through all triangles
//     for (size_t Index = 0; Index < Indices.size(); Index += 3)
//     {
//         // Transform vertices using the model matrix
//         glm::vec3 Vertex0 = glm::vec3(ModelMatrix * glm::vec4(this->GetVertexPositions(Vertices,Index, Stride), 1.0f));
//         glm::vec3 Vertex1 = glm::vec3(ModelMatrix * glm::vec4(this->GetVertexPositions(Vertices,Index+1, Stride), 1.0f));
//         glm::vec3 Vertex2 = glm::vec3(ModelMatrix * glm::vec4(this->GetVertexPositions(Vertices,Index+2, Stride), 1.0f));

//         // Compute the normal of the triangle
//         glm::vec3 Edge1 = Vertex1 - Vertex0;
//         glm::vec3 Edge2 = Vertex2 - Vertex0;
//         glm::vec3 Normal = glm::normalize(glm::cross(Edge1, Edge2));

//         // Check if the point is within the plane of the triangle
//         float Distance = glm::dot(Normal, Point - Vertex0);
//         if (glm::abs(Distance) > Threshold)
//         {
//             continue; // Point is too far from the triangle plane
//         }

//         // Barycentric coordinate method for point-in-triangle test
//         glm::vec3 v0 = Vertex1 - Vertex0;
//         glm::vec3 v1 = Vertex2 - Vertex0;
//         glm::vec3 v2 = Point - Vertex0;

//         float d00 = glm::dot(v0, v0);
//         float d01 = glm::dot(v0, v1);
//         float d11 = glm::dot(v1, v1);
//         float d20 = glm::dot(v2, v0);
//         float d21 = glm::dot(v2, v1);
//         float denom = d00 * d11 - d01 * d01;

//         float v = (d11 * d20 - d01 * d21) / denom;
//         float w = (d00 * d21 - d01 * d20) / denom;
//         float u = 1.0f - v - w;

//         // If all barycentric coordinates are between 0 and 1, the point is inside the triangle
//         if (u >= 0.0f && v >= 0.0f && w >= 0.0f)
//         {
//             return true;
//         }
//     }

//     return false; // No intersection found
// }

void Collisions::CleanUp()
{
    for (int Index = 0; Index < this->CachedElementBounds.size(); Index++)
    {if (this->CachedElementBounds[Index]) {delete this->CachedElementBounds[Index];}}
}