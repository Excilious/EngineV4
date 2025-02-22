#include "Frame.h"
#include "../../Common/Console.h"

Frame::Frame(glm::vec2 Position,glm::vec2 Scale,int Width,int Height,glm::vec3 Colour,float Rotation,std::string FrameVertex,std::string FrameFragment)
{   
    this->NewShader     = new Shader();
    this->Rotation      = Rotation;
    this->Scale         = Scale;
    this->Position      = Position;
    this->Colour        = Colour;
    this->WindowWidth   = Width;
    this->WindowHeight  = Height;

    float Vertices[ ] = { 
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    this->NewShader->CreateNewShader(FrameVertex.c_str(),FrameFragment.c_str(),true);
    glGenVertexArrays(1,&this->FrameVertexArray);
    glGenBuffers(1,&this->FrameVertexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER,this->FrameVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(Vertices),Vertices,GL_STATIC_DRAW);

    glBindVertexArray(this->FrameVertexArray);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    this->NewShader->ActivateShader();
}

void Frame::RenderInstance()
{
    glDisable(GL_DEPTH_TEST);
    this->NewShader->ActivateShader();
    this->ModelMatrix = glm::mat4(1.0f);
    this->ModelMatrix = glm::translate(this->ModelMatrix,glm::vec3(this->Position,0.0f));
    this->ModelMatrix = glm::translate(this->ModelMatrix,glm::vec3(0.5f*this->Scale.x,0.5f*this->Scale.y,0.0f));
    this->ModelMatrix = glm::rotate(this->ModelMatrix,glm::radians(this->Rotation),glm::vec3(0.0f,0.0f,1.0f));
    this->ModelMatrix = glm::translate(this->ModelMatrix,glm::vec3(-0.5f*this->Scale.x,-0.5f*this->Scale.y,0.0f));
    this->ModelMatrix = glm::scale(this->ModelMatrix,glm::vec3(this->Scale,1.0f)); 

    this->ProjectionMatrix = glm::ortho(0.0f,this->WindowWidth,this->WindowHeight,0.0f,-1.0f,1.0f);
    glUniformMatrix4fv(glGetUniformLocation(this->NewShader->Program,"model"),1,GL_FALSE,glm::value_ptr(this->ModelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(this->NewShader->Program,"projection"),1,GL_FALSE,glm::value_ptr(this->ProjectionMatrix));
    glUniform3f(glGetUniformLocation(this->NewShader->Program,"spriteColor"),this->Colour.x,this->Colour.y,this->Colour.z);

    if (this->IsFrameImage) {glUniform1f(glGetUniformLocation(this->NewShader->Program,"IsImage"),1.0f);}
    else                    {glUniform1f(glGetUniformLocation(this->NewShader->Program,"IsImage"),0.0f);}
  
    glBindVertexArray(this->FrameVertexArray);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
}

void Frame::DestroyInstance()
{
    glDeleteVertexArrays(1,&this->FrameVertexArray);
    this->NewShader->DestroyShader();
    if (this->NewShader) {delete this->NewShader;}
}
