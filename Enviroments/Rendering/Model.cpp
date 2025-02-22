#include "Model.h"
#include "../Common/Console.h"

Model::Model(std::string ModelName,std::string ModelFile,Parser* NewParser,glm::vec3 Position,glm::quat Rotation,glm::vec3 Scale): Mesh(Position,Rotation,Scale)
{
    this->Contents          = NewParser->GetGlobalDatapackData(ModelFile);
    this->Json              = nlohmann::json::parse(this->Contents);
    this->ModelName         = ModelName;
    this->TranslationModel  = Position;
    this->RotationModel     = Rotation;
    this->ScaleModel        = Scale;
    this->NewData           = NewParser->GetURIData(this->Contents);
    this->TraverseModel(0);
}

std::vector<glm::vec2> Model::GroupVector2(std::vector<float> FloatVector)
{
    std::vector<glm::vec2> Vectors;
    for (int Index = 0; Index < FloatVector.size(); Index)
    {Vectors.push_back(glm::vec2(FloatVector[Index++],FloatVector[Index++]));}
    return Vectors;
}

std::vector<glm::vec3> Model::GroupVector3(std::vector<float> FloatVector)
{
    std::vector<glm::vec3> Vectors;
    for (int Index = 0; Index < FloatVector.size(); Index)
    {Vectors.push_back(glm::vec3(FloatVector[Index++],FloatVector[Index++],FloatVector[Index++]));}
    return Vectors;
}

std::vector<glm::vec4> Model::GroupVector4(std::vector<float> FloatVector)
{
    std::vector<glm::vec4> Vectors;
    for (int Index = 0; Index < FloatVector.size(); Index)
    {Vectors.push_back(glm::vec4(FloatVector[Index++],FloatVector[Index++],FloatVector[Index++],FloatVector[Index++]));}
    return Vectors;
}

std::vector<Vertex> Model::AssembleVertices(std::vector<glm::vec3> Position,std::vector<glm::vec3> Normal,std::vector<glm::vec2> TextureUV)
{
    std::vector<Vertex> Vertices;
    for (int Index = 0; Index < Position.size(); Index++)
    {Vertices.push_back(Vertex{Position[Index],Normal[Index],glm::vec3(1.0f,1.0f,1.0f),TextureUV[Index]});}
    return Vertices;
}

std::vector<float> Model::GetFloats(nlohmann::json Accessor)
{
    unsigned int BufferViewIndex        = Accessor.value("bufferView",1);
    unsigned int Count                  = Accessor["count"];
    unsigned int AccessorByteOffset     = Accessor.value("byteOffset",0);

    std::vector<float> FloatVector;
    std::string Type                    = Accessor["type"];
    nlohmann::json BufferView           = this->Json["bufferViews"][BufferViewIndex];
    unsigned int ByteOffset             = BufferView["byteOffset"];
    unsigned int NumberPerVertices;

    if (Type == "SCALAR")       {NumberPerVertices=1;}
    else if (Type == "VEC2")    {NumberPerVertices=2;}
    else if (Type == "VEC3")    {NumberPerVertices=3;}
    else if (Type == "VEC4")    {NumberPerVertices=4;}
    else {SysPrint("Vector Doesn't Exist.",2);}

    unsigned int StartOfData    = ByteOffset + AccessorByteOffset;
    unsigned int LengthOfData   = Count * 4 * NumberPerVertices;

    for (unsigned int Index = StartOfData; Index < StartOfData+LengthOfData; Index)
    {
        float Value;
        unsigned char Bytes[] = {this->NewData[Index++],this->NewData[Index++],this->NewData[Index++],this->NewData[Index++]};
        std::memcpy(&Value,Bytes,sizeof(float)); 
        FloatVector.push_back(Value);
    }
    return FloatVector;
}

std::vector<GLuint> Model::GetIndices(nlohmann::json Accessor)
{
    std::vector<GLuint> Indices;
    unsigned int BufferViewIndices      = Accessor.value("bufferView",0);
    unsigned int Count                  = Accessor["count"];
    unsigned int AccessorByteOffset     = Accessor.value("byteOffset",0);
    unsigned int ComponentType          = Accessor["componentType"];

    nlohmann::json BufferView           = this->Json["bufferViews"][BufferViewIndices];
    unsigned int ByteOffset             = BufferView["byteOffset"];
    unsigned int StartOfData            = ByteOffset+AccessorByteOffset;

    if (ComponentType == 5125)
    {
        for (unsigned int Index = StartOfData; Index < ByteOffset+AccessorByteOffset+Count*4; Index)
        {
            unsigned char Bytes[] = {this->NewData[Index++],this->NewData[Index++],this->NewData[Index++],this->NewData[Index++]};
            unsigned int Value;
            std::memcpy(&Value,Bytes,sizeof(unsigned int));
            Indices.push_back((GLuint)Value);
        }
    }
    else if (ComponentType == 5123)
    {
        for (unsigned int Index = StartOfData; Index < ByteOffset+AccessorByteOffset+Count*2; Index)
        {
            unsigned char Bytes[] = {this->NewData[Index++],this->NewData[Index++]};
            unsigned short Value;
            std::memcpy(&Value,Bytes,sizeof(unsigned short));
            Indices.push_back((GLuint)Value);
        }
    }
    else if (ComponentType == 5122)
    {
        for (unsigned int Index = StartOfData; Index < ByteOffset+AccessorByteOffset+Count*2; Index)
        {
            unsigned char Bytes[] = {this->NewData[Index++],this->NewData[Index++]};
            short Value;
            std::memcpy(&Value,Bytes,sizeof(short));
            Indices.push_back((GLuint)Value);
        }
    }
    return Indices;
}

std::vector<Texture*> Model::GetTexture()
{
    std::vector<Texture*>   Textures;
    // std::string             FileString      = std::string(this->FileName);
    // std::string             FileDirectory   = FileString.substr(0,FileString.find_last_of("/")+1);

    // for (unsigned int Index = 0; Index < this->Json["images"].size(); Index++)
    // {
    //     std::string TexturePath = this->Json["images"][Index]["uri"];
    //     bool Skip               = false;
        
    //     for (unsigned int TextureIndex = 0; TextureIndex < LoadedTextureName.size(); TextureIndex++)
    //     {
    //         if (LoadedTextureName[TextureIndex] == TexturePath)
    //         {
    //             Textures.push_back(LoadedTexture[TextureIndex]);
    //             Skip = true;
    //             break;
    //         }
    //     }
    //     if (!Skip)
    //     {
    //         if (TexturePath.find("baseColor") != std::string::npos)
    //         {
    //             Texture *Diffuse;
    //             Diffuse->CreateNormalTexture((FileDirectory+TexturePath).c_str(),"diffuse",LoadedTexture.size());
    //             Textures.push_back(Diffuse);
    //             LoadedTexture.push_back(Diffuse);
    //         }
    //         else if (TexturePath.find("metallicRoughness") != std::string::npos)
    //         {
    //             Texture *Specular;
    //             Specular->CreateNormalTexture((FileDirectory+TexturePath).c_str(),"specular",LoadedTexture.size());
    //             Textures.push_back(Specular);
    //             LoadedTexture.push_back(Specular);
    //         }
    //         LoadedTextureName.push_back(TexturePath);
    //     }
    // }
    return Textures;
}

void Model::DrawModel(Shader* NewShader,Camera* NewCamera,float NearPlane,float FarPlane,float FieldOfView)
{this->Draw(NewShader,NewCamera,NearPlane,FarPlane,FieldOfView);}

void Model::TraverseModel(unsigned int NextNode)
{
    nlohmann::json NewNode = this->Json["nodes"][NextNode];
    if (NewNode.find("mesh") != NewNode.end())      {this->LoadModel(NewNode["mesh"]);}
    if (NewNode.find("children") != NewNode.end())  {for (unsigned int Index = 0; Index < NewNode["children"].size(); Index++) {this->TraverseModel(NewNode["children"][Index]);}}
}

void Model::LoadModel(unsigned int MeshIndex)
{
    unsigned int PositionAccessorIndex      = this->Json["meshes"][MeshIndex]["primitives"][0]["attributes"]["POSITION"];
    unsigned int NormalAccessorIndex        = this->Json["meshes"][MeshIndex]["primitives"][0]["attributes"]["NORMAL"];
    unsigned int TextureAccessorIndex       = this->Json["meshes"][MeshIndex]["primitives"][0]["attributes"]["TEXCOORD_0"];
    unsigned int IndicesAccessorIndex       = this->Json["meshes"][MeshIndex]["primitives"][0]["indices"];

    std::vector<float> PositionVector       = this->GetFloats(this->Json["accessors"][PositionAccessorIndex]);
    std::vector<float> NormalVector         = this->GetFloats(this->Json["accessors"][NormalAccessorIndex]);
    std::vector<float> TextureVector        = this->GetFloats(this->Json["accessors"][TextureAccessorIndex]);
    this->Positions                         = this->GroupVector3(PositionVector);
    this->Normals                           = this->GroupVector3(NormalVector);
    this->TextureUV                         = this->GroupVector2(TextureVector);

    std::vector<Vertex> Vertices        = this->AssembleVertices(Positions,Normals,TextureUV);
    std::vector<GLuint> Indices         = this->GetIndices(this->Json["accessors"][IndicesAccessorIndex]);
    std::vector<Texture*> Textures      = this->GetTexture();

    this->Generate(Vertices,Indices,Textures);
}

void Model::DestroyModel()
{
    this->VertexArray.Delete();
    this->Destroy();
}