#ifndef __LIGHTING_HEADER__
#define __LIGHTING_HEADER__

#include "Model.h"
typedef enum 
{
    LIGHTDEF_POINT
} LightType;

class Lighting: public Model
{
    public:
        Lighting(std::string ModelName,std::string ModelFile,Parser* NewParser,glm::vec3 Position,glm::quat Rotation,glm::vec3 Scale);
        void CreateLight(glm::vec3 Colour);
        float       Brightness;
        float       Gloss;
        float       Transparency;
        glm::vec3   Colours;
        LightType   TypeOfLight;
};

#endif