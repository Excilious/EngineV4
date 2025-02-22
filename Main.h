#define VERSION_MINOR 3
#define VERSION_MAJOR 3
#define MSAA_SAMPLES  8

#include <windows.h>
#include <iostream>
#include <string>
#include <math.h>
#include <tchar.h>
#include <thread>
#include "Dependancies/glad/glad.h"
#include "Dependancies/GLFW/glfw3.h"
#include "Dependancies/glm/glm.hpp"

#include "Enviroments/Rendering/Skybox.h"
#include "Enviroments/Rendering/Interface/Frame.h"
#include "Enviroments/Rendering/Interface/Label.h"
#include "Enviroments/World/World.h"
#include "Enviroments/Common/Console.h"
#include "Enviroments/Audio/Audio.h"

class Main
{
    public:
        Main();
        void CalculateFramesRendered();
        void Render(GLFWwindow* EngineWindow);
        void Release();
        void Initilise();
        void LoadWorld();
        void Destroy();

        int                 Width                   = 1920;
        int                 Height                  = 1080;
        float               NearView                = 0.1f;
        float               FarView                 = 100.0f;
        float               FieldOfView             = 90.0f;
        double              PreviousTime            = 0.0;
        double              CurrentTime             = 0.0;
        unsigned int        Counter                 = 0;
        bool                InitilisationComplete   = false;
        bool                StartWorld              = false;
        std::string         WorldInstance           = "-1";

        double              DeltaTime;
        double              XPosition;
        double              YPosition;
        GLFWwindow*         EngineWindow;
        std::string         FramesPerSecond;
    private:
        Player*             PlayerWorld;
        Label*              FramePerSecondLabel;
        Audio*              NewSound;
        Parser*             EngineParser;
        Shader*             SkyboxShader;
        Skybox*             EngineSkybox;
        World*              EngineWorld;
};

