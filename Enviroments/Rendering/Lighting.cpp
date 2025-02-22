#include "Lighting.h"

Lighting::Lighting(std::string ModelName,std::string ModelFile,Parser* NewParser,glm::vec3 Position,glm::quat Rotation,glm::vec3 Scale) : Model(ModelName,ModelFile,NewParser,Position,Rotation,Scale) {};

void Lighting::CreateLight(glm::vec3 Colour)
{
    this->Colours       = Colour;
    this->TypeOfLight   = LIGHTDEF_POINT;
}