#include "World.h"

World::World()
{}

std::vector<float> World::GetSceneVertices()
{
    std::vector<float> Vertices;
    if (this->VertexCache.find(this->LevelIndex) != this->VertexCache.end()) {return this->VertexCache[this->LevelIndex];}

    std::string VerticesData  = this->CurrentParser->GetGlobalDatapackData(this->NewJson["MapData"][this->LevelIndex]["Vertices"]);
    std::istringstream StringStream(VerticesData);
    std::string NewToken;
    float Vertex;
    if (VerticesData == "")          {SysPrint("Failed to get vertices data for map index"+std::to_string(this->LevelIndex),2); return Vertices;}
    while (std::getline(StringStream,NewToken,','))
    {
        try                                     {Vertices.push_back(std::stof(NewToken));} 
        catch (const std::invalid_argument&)    {SysPrint("Invalid or NAN for vertex "+NewToken,2);};
    }

    if (this->VertexCache.find(this->LevelIndex) == this->VertexCache.end()) {this->VertexCache.insert({this->LevelIndex,Vertices});}
    return Vertices;
}

std::vector<unsigned int> World::GetSceneIndices()
{
    std::vector<unsigned int> Indices; 
    if (this->IndicesCache.find(this->LevelIndex) != this->IndicesCache.end()) {return this->IndicesCache[this->LevelIndex];}

    std::string IndicesData = this->CurrentParser->GetGlobalDatapackData(this->NewJson["MapData"][this->LevelIndex]["Indices"]);
    std::istringstream StringStream(IndicesData);
    std::string NewToken;
    unsigned int Index;
    if (IndicesData == "")          {SysPrint("Failed to get indices data for map index"+std::to_string(this->LevelIndex),2); return Indices;}
    while (std::getline(StringStream,NewToken,','))
    {
        try                                     {Indices.push_back(std::stoul(NewToken));} 
        catch (const std::invalid_argument&)    {SysPrint("Invalid or NAN for index "+NewToken,2);};
    }

    if (this->IndicesCache.find(this->LevelIndex) == this->IndicesCache.end())  {this->IndicesCache.insert({this->LevelIndex,Indices});}
    return Indices;
}

void World::GenerateWorld(Parser* WorldParser)
{
    this->WorldTerrain          = new Terrain();
    this->WorldWater            = new Water();
    this->SceneShaderInstance   = new Shader();
    this->NewInstance           = new Instance();
    this->NewLevelScene         = new Scene();
    this->SceneCollider         = new Collisions();

    this->CollisionShader       = new Shader();
    this->WorldCollisionShape   = new CollisionShape();


    this->CurrentParser         = WorldParser;

    this->SceneShaderInstance->CreateNewShader(WorldParser->GetGlobalDatapackData("Scene.vert").c_str(),WorldParser->GetGlobalDatapackData("Scene.frag").c_str(),true);
    if (WorldParser->GetGlobalDatapackData("World.canv") == "") {SysPrint("World.canv isnt found within the datapack or is corrupt.",3); return;}
    std::string     CanvasList = WorldParser->GetGlobalDatapackData("World.canv");
    unsigned int    CanvasType = WorldParser->QueryDataTypeContent("World.canv");
    if (CanvasType == DT_CANVAS) {this->NewJson = nlohmann::json::parse(CanvasList);}

    for (int Index = 0; Index < this->NewJson["Entity"].size(); Index++)
    {
        glm::vec3 Position  = glm::vec3(this->NewJson["Entity"][Index]["Position"][0],this->NewJson["Entity"][Index]["Position"][1],this->NewJson["Entity"][Index]["Position"][2]);
        glm::vec3 Scale     = glm::vec3(this->NewJson["Entity"][Index]["Scale"][0],this->NewJson["Entity"][Index]["Scale"][1],this->NewJson["Entity"][Index]["Scale"][2]);
        glm::quat Rotation  = glm::quat(this->NewJson["Entity"][Index]["Rotation"][0],this->NewJson["Entity"][Index]["Rotation"][1],this->NewJson["Entity"][Index]["Rotation"][2],this->NewJson["Entity"][Index]["Rotation"][3]);

        this->NewInstance->GenerateStaticEntity(WorldParser,this->NewJson["Entity"][Index]["MeshReference"],WorldParser->GetGlobalDatapackData(this->NewJson["Entity"][Index]["ShaderVert"]),WorldParser->GetGlobalDatapackData(this->NewJson["Entity"][Index]["ShaderFrag"]),Position,Scale,Rotation);
    }

    glm::vec3 Position      = glm::vec3(this->NewJson["MapData"][this->LevelIndex]["Position"][0],this->NewJson["MapData"][this->LevelIndex]["Position"][1],this->NewJson["MapData"][this->LevelIndex]["Position"][2]);
    glm::quat Rotation      = glm::quat(this->NewJson["MapData"][this->LevelIndex]["Rotation"][0],this->NewJson["MapData"][this->LevelIndex]["Rotation"][1],this->NewJson["MapData"][this->LevelIndex]["Rotation"][2],this->NewJson["MapData"][this->LevelIndex]["Rotation"][3]);
    glm::vec3 Scale         = glm::vec3(this->NewJson["MapData"][this->LevelIndex]["Scale"][0],this->NewJson["MapData"][this->LevelIndex]["Scale"][1],this->NewJson["MapData"][this->LevelIndex]["Scale"][2]);

    this->TranslationMatrix         = glm::translate(glm::mat4(1.0f),Position);
    this->RotationMatrix            = glm::mat4_cast(Rotation);
    this->ScaleMatrix               = glm::scale(glm::mat4(1.0f),Scale); 
    this->CollisionRotationMatrix   = this->RotationMatrix;
    // this->CollisionRotationMatrix   = glm::rotate(this->CollisionRotationMatrix,360.0f,glm::vec3(1.0f,0.0f,0.0f));

    if (this->NewJson["MapData"][this->LevelIndex]["UseGLTF"] == "1")
    {this->NewLevelScene->LoadFromGLTF(WorldParser,this->NewJson["MapData"][this->LevelIndex]["GLTFFile"],this->TranslationMatrix,this->RotationMatrix,this->ScaleMatrix);}
    else
    {this->NewLevelScene->GenerateLevelParitionData(this->GetSceneVertices(),this->GetSceneIndices(),this->TranslationMatrix,this->RotationMatrix,this->ScaleMatrix);}

    this->WorldTerrain->GenerateHeightMap();
    this->WorldTerrain->GenerateTerrainMesh(WorldParser);
    this->WorldTerrain->GenerateTerrainBuffers();

    this->WorldWater->GenerateWaterVertices(WorldParser);
    this->WorldWater->GenerateWaterBuffers();

    this->CollisionShader->CreateNewShader("../../Shaders/Collision.vert","../../Shaders/Collision.frag",false);

    this->WorldCollisionShape->GenerateCollisionShapeFromVertex(this->NewLevelScene->Vertices,this->NewLevelScene->Indices,(this->TranslationMatrix*this->CollisionRotationMatrix*this->ScaleMatrix),3);
    this->WorldCollisionShape->RenderGeneratePhysicsCollisionShape();
} 

void World::RenderWorld(Camera *EngineCamera,float Near,float Far,float FieldOfView,unsigned int ReflectanceTexture)
{
    //EngineCamera->Position.y = this->WorldTerrain->GetNextPlayerPosition(EngineCamera->Position).y;
    //this->WorldCollisionShape->RenderPhysicsCollisionShape(this->CollisionShader->Program,EngineCamera->CameraMatrix);
    //this->NewLevelScene->DrawScene(this->SceneShaderInstance,EngineCamera,Near,Far,FieldOfView);
    this->WorldTerrain->RenderTerrain(EngineCamera);
    this->WorldWater->RenderWater(EngineCamera,ReflectanceTexture);
    this->NewInstance->RenderEntities(EngineCamera,Near,Far,FieldOfView);
}

void World::DestroyWorld()
{
    this->NewLevelScene->DestroyScene();
    this->NewInstance->DestroyAll();
    this->WorldTerrain->Destroy();
    this->WorldWater->Destroy();
    this->SceneShaderInstance->DestroyShader();
    this->SceneCollider->CleanUp();

    if (this->WorldTerrain)         {delete this->WorldTerrain;}
    if (this->SceneShaderInstance)  {delete this->SceneShaderInstance;}
    if (this->NewInstance)          {delete this->NewInstance;}
    if (this->NewLevelScene)        {delete this->NewLevelScene;}
    if (this->SceneCollider)        {delete this->SceneCollider;}
    if (this->WorldWater)           {delete this->WorldWater;}
}