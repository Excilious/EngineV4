#ifndef __INTERFACE_H__
#define __INTERFACE_H__
#define GLT_IMPLEMENTATION

#include <vector>
#include <map>
#include <string>
#include "../Dependancies/glad/glad.h"
#include "../Dependancies/glm/glm.hpp"
#include "../Dependancies/GLFW/glfw3.h"
#include "../Dependancies/gltext/gltext.h"

#include "../Dependancies/glm/gtc/matrix_transform.hpp"
#include "../Dependancies/glm/gtc/type_ptr.hpp"
#include "../../Common/Shader.h"

class Frame
{
    public:
        Frame(glm::vec2 Position,glm::vec2 Scale,int Width,int Height,glm::vec3 Colour,float Rotation,std::string FrameVertex,std::string FrameFragment);
        void RenderInstance();
        void DestroyInstance();

        float               WindowWidth;
        float               WindowHeight;
        float               Rotation;
        bool                IsFrameImage = false;
        glm::vec2           Position;
        glm::vec2           Scale;
        glm::vec3           Colour;
        glm::mat4           OrthoMatrix;
        std::string         FileName;
        Shader*             NewShader;

    private:
        unsigned int        TextureID;
        unsigned int        FrameVertexArray;
        unsigned int        FrameVertexBuffer;
        glm::mat4           ProjectionMatrix;
        glm::mat4           ModelMatrix;
};

#endif