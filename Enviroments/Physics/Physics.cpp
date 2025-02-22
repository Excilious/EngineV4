#include "Physics.h"
#include "../Common/Console.h"

Physics::Physics()
{}

void Physics::SetEntityVelocity(int EntityIndex)
{
    if (this->VelocityTable.find(EntityIndex) == this->VelocityTable.find(EntityIndex)) {this->VelocityTable.insert({EntityIndex,0.0f});}
}

glm::vec3 Physics::RenderEntityGravity(int EntityIndex,float DeltaTime)
{
    glm::vec3 NewVector = glm::vec3(0.0f,0.0f,0.0f);
    if (this->VelocityTable.find(EntityIndex) != this->VelocityTable.end())
    {
        float Time  = (float)(DeltaTime/1e3);
        NewVector.y = (this->VelocityTable.find(EntityIndex)->second)*Time + 0.5*9.81*(Time*Time);
        std::cout << NewVector.y << "\n";
    }
    else {SysPrint("Missing Velocity Of Object Index "+std::to_string(EntityIndex),2);}
    return NewVector;
}