#include "Shader.h"
#include "../Common/Console.h"

Shader::Shader() {}

std::string GetShaderContents(const char* ShaderName)
{
    std::ifstream ShaderFile(ShaderName, std::ios::binary);
    if (ShaderFile.is_open())
    {
        std::string Contents;
        ShaderFile.seekg(0,std::ios::end);
        Contents.resize(ShaderFile.tellg());
        ShaderFile.seekg(0,std::ios::beg);
        ShaderFile.read(&Contents[0],Contents.size());
        ShaderFile.close();
        return (Contents);
    }
    return "";
}

void ReportErrors(unsigned int Shader,int Type)
{
    GLint HasCompiled;
    char InfoLog[1024];
    if (Type != PROGRAM_REPORT_ID)
    {
        glGetShaderiv(Shader,GL_COMPILE_STATUS,&HasCompiled);
        if (HasCompiled == 0)
        {
            glGetShaderInfoLog(Shader,1024,NULL,InfoLog);
            SysPrint("",2);
            SysPrint("Failed To Compile: "+(std::string)InfoLog,2);
            SysPrint("Type Of Shader: "+(std::string)(Type == VERTEX_REPORT_ID ? "Vertex" : (Type == FRAGMENT_REPORT_ID ? "Fragment" : "Unknown")),2);
        }
    } else {
        glGetProgramiv(Shader,GL_LINK_STATUS,&HasCompiled);
        if (HasCompiled == 0)
        {
            glGetProgramInfoLog(Shader,1024,NULL,InfoLog);
            SysPrint("",2);
            SysPrint("Failed To Compile: "+(std::string)InfoLog,2);
            SysPrint("Type Of Shader: Program",2);
        }
    }
}

void Shader::CreateNewShader(const char* VertexShaderFile,const char* FragmentShaderFile,bool Source)
{
    GLenum      ProgramError;
    std::string VertexText;
    std::string FragmentText;

    if (!Source)    {VertexText = GetShaderContents(VertexShaderFile); FragmentText = GetShaderContents(FragmentShaderFile);}
    else            {VertexText = VertexShaderFile, FragmentText = FragmentShaderFile;}

    const char* VertexSource = VertexText.c_str();
    const char* FragmentSource = FragmentText.c_str();

    GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(VertexShader,1,&VertexSource,NULL);
    glShaderSource(FragmentShader,1,&FragmentSource,NULL);

    glCompileShader(VertexShader);
    glCompileShader(FragmentShader);
    ReportErrors(VertexShader,VERTEX_REPORT_ID);
    ReportErrors(FragmentShader,FRAGMENT_REPORT_ID);

    this->Program = glCreateProgram();
    glAttachShader(this->Program,VertexShader);
    glAttachShader(this->Program,FragmentShader);
    glLinkProgram(this->Program);
    ReportErrors(this->Program,PROGRAM_REPORT_ID);

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
}

void Shader::ActivateShader()
{glUseProgram(this->Program);};

void Shader::DestroyShader()
{glDeleteProgram(this->Program);};

