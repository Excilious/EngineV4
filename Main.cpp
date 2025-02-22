#include "Main.h"

Main::Main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES,MSAA_SAMPLES);

    GLFWwindow* EngineWindow = glfwCreateWindow(this->Width,this->Height,"Fusion",glfwGetPrimaryMonitor(),NULL);
    glfwMakeContextCurrent(EngineWindow);
    gladLoadGL();
    glViewport(0,0,this->Width,this->Height);
    
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glCullFace(GL_FRONT);
    glfwSwapInterval(1);
    glFrontFace(GL_CCW);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    if (!glfwInit())                                            {MessageBox(NULL,TEXT("Failed to load GLFW window."),TEXT("Fusion Engine"),MB_ICONERROR|MB_OK); glfwTerminate();}
    if (EngineWindow == NULL)                                   {glfwTerminate();} 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))    {glfwTerminate();}
    this->EngineWindow          = EngineWindow;
    this->FramePerSecondLabel   = new Label("-1",glm::vec2(0.0f,1050.0f),1.0f,glm::vec3(1.0f,1.0f,1.0f));
}

void OnWindowResizedCallback(GLFWwindow* EngineWindow,int Width,int Height) {glViewport(0,0,Width,Height);}
void Main::Release()                                                        {if (glfwGetKey(this->EngineWindow,GLFW_KEY_ESCAPE) == GLFW_PRESS) {glfwTerminate(); exit(0);}}

void Main::CalculateFramesRendered()
{
    this->CurrentTime = glfwGetTime();
    this->DeltaTime = this->CurrentTime - this->PreviousTime;
    this->Counter++;

    if (this->DeltaTime >= 1.0f / 30.0)
    {
        this->FramesPerSecond                   = std::to_string((1.0/this->DeltaTime) * this->Counter);
        this->PreviousTime                      = this->CurrentTime;
        this->Counter                           = 0;
        this->FramePerSecondLabel->TextContent  = this->FramesPerSecond;
    }
}

void Main::LoadWorld()
{
    this->WorldInstance = "../../Fusion/Base.wrdx";
    std::ifstream BaseFile(this->WorldInstance);
    if (!BaseFile.good())
    {
        MessageBox(NULL,TEXT("Cannot find Base.wrdx! Ensure that the base file is within the correct directory."),TEXT("Fusion Engine"),MB_ICONERROR|MB_OK);
        glfwTerminate();
    }
    else {BaseFile.close();}

    this->EngineParser  = new Parser();
    this->EngineParser->FetchFromDatapack(this->WorldInstance);
}

void Main::Initilise()
{
    //this->NewSound->PlayLocalSound("../../Sounds/Evil.wav");
    if (this->WorldInstance != "-1")
    {
        this->SkyboxShader  = new Shader();
        this->PlayerWorld   = new Player(this->Width,this->Height,glm::vec3(20.0f,10.0f,0.0f));
        this->EngineSkybox  = new Skybox(this->Width,this->Height,this->NearView,this->FarView,this->EngineParser,this->FieldOfView);
        this->EngineWorld   = new World();

        glfwSetWindowSizeCallback(this->EngineWindow,OnWindowResizedCallback);

        this->SkyboxShader->CreateNewShader(
            this->EngineParser->GetGlobalDatapackData("Skybox.vert").c_str(),
            this->EngineParser->GetGlobalDatapackData("Skybox.frag").c_str(),
            true
        );
        this->SkyboxShader->ActivateShader();
        this->EngineWorld->GenerateWorld(this->EngineParser);
        this->InitilisationComplete = true;
    }
}

void Main::Destroy()
{
    CommandDelete();
    this->SkyboxShader->DestroyShader();
    this->EngineWorld->DestroyWorld();
    this->EngineSkybox->Destroy();
    if (this->EngineParser) {delete this->EngineParser;}
    if (this->SkyboxShader) {delete this->SkyboxShader;}
    if (this->PlayerWorld)  {delete this->PlayerWorld;}
    if (this->EngineSkybox) {delete this->EngineSkybox;}
    if (this->EngineWorld)  {delete this->EngineWorld;}
    glfwDestroyWindow(this->EngineWindow);
    glfwTerminate();
}

void Main::Render(GLFWwindow* EngineWindow)
{
    this->LoadWorld();
    CommandGenerateBackFrame(this->Width,this->Height,
        this->EngineParser->GetGlobalImageBuffer("DragImageFrame.imgbuf"),
        this->EngineParser->GetGlobalImageBuffer("ConsoleImageFrame.imgbuf"),
        this->EngineParser->GetGlobalDatapackData("Frame.vert"),
        this->EngineParser->GetGlobalDatapackData("Frame.frag")
    );

    SysPrint("This application is in beta!",2);
    SysPrint("Fusion Engine, GL Version "+std::to_string(VERSION_MAJOR)+std::to_string(VERSION_MINOR),1);
    SysPrint("Resolution: "+std::to_string(this->Width)+"x"+std::to_string(this->Height),1);

    this->Initilise();
    while (!glfwWindowShouldClose(this->EngineWindow))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f,0.0f,0.0f,1.0f);

        if (this->WorldInstance != "-1" && this->InitilisationComplete)
        {
            this->PlayerWorld->CheckPlayerMovementInputs(this->EngineWindow,this->EngineWorld->NewLevelScene->Vertices,this->EngineWorld->NewLevelScene->Indices,(
                this->EngineWorld->TranslationMatrix*
                this->EngineWorld->RotationMatrix*
                this->EngineWorld->ScaleMatrix
            ));
            this->PlayerWorld->UpdateMatrix(this->FieldOfView,this->NearView,this->FarView);
            this->PlayerWorld->UpdateCameraPosition(this->XPosition,this->YPosition);
            this->EngineWorld->RenderWorld(this->PlayerWorld,this->NearView,this->FarView,this->FieldOfView,this->EngineSkybox->CubemapTexture);
            this->EngineSkybox->RenderSkybox(this->PlayerWorld->Position,this->PlayerWorld->Orientation,this->PlayerWorld->UpPosition,this->SkyboxShader);
            this->FramePerSecondLabel->RenderText();
        }

        CommandUpdateInput(this->EngineWindow);
        CommandRenderText(this->EngineWindow);
        glfwGetCursorPos(this->EngineWindow,&XPosition,&YPosition);
        glfwSwapBuffers(this->EngineWindow);

        this->Release();
        this->CalculateFramesRendered();
        glfwPollEvents();
    }
}

int main()
{
    Main Engine;
    Engine.Render(Engine.EngineWindow);
    Engine.Destroy();    
}