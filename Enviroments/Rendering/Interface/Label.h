#ifndef __LABEL_HEADER__
#define __LABEL_HEADER__
#define GLT_IMPLEMENTATION

#include <string>
#include "../Dependancies/glad/glad.h"
#include "../Dependancies/glm/glm.hpp"
#include "../Dependancies/GLFW/glfw3.h"
#include "../Dependancies/gltext/gltext.h"

class Label
{
    public:
        Label(std::string Text,glm::vec2 TextPosition,float TextScale,glm::vec3 TextColour);
        void RenderText();
        void DestroyText();
        std::string     TextContent;
        glm::vec2       TextPosition;
        glm::vec3       TextColour;
    private:
        float           Scale;
        GLTtext         *TextInstance;
        unsigned int    TextVertexArray;
        unsigned int    TextVertexBuffer;
};

#endif