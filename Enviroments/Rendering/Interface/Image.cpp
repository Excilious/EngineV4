#include "Image.h"
#include "../Texture.h"

Image::Image(unsigned char* ImageBuffer,glm::vec2 Position,glm::vec2 Scale,int Width,int Height,float Rotation,std::string FrameVertex,std::string FrameFragment): Frame(Position,Scale,Width,Height,glm::vec3(0.0f,0.0f,0.0f),Rotation,FrameVertex,FrameFragment)
{
    Texture* Diffuse = new Texture();
    glBindTexture(GL_TEXTURE_2D,0);
    Diffuse->CreateBufferTexture(ImageBuffer,Width,Height);
    Diffuse->TextureUnit(this->NewShader,"TextureID",4);
    Diffuse->Bind();
    this->IsFrameImage = true;
    this->NewShader->ActivateShader();
}