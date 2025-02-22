#ifndef __IMAGE_HEADER__
#define __IMAGE_HEADER__

#include "Frame.h"

class Image: public Frame
{
    public:
        Image(unsigned char* ImageBuffer,glm::vec2 Position,glm::vec2 Scale,int Width,int Height,float Rotation,std::string FrameVertex,std::string FrameFragment);
    private:
};

#endif