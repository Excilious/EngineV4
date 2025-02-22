#include "Console.h"

typedef struct CommandFunctions
{
    Function        ExecutableFunction;
    std::string     CommandString;
} Command;

bool                        IsConsoleOpen   = true;

double                      LastKeyTime     = 0.0;
double                      CurrentKeyTime  = 0.0;
double                      LastOpenTime    = 0.0;
double                      CurrentOpenTime = 0.0;
float                       PositionY       = 0.0f;
std::vector<Label*>         Console         = {};
std::vector<glm::vec2>      PositionCache;
std::vector<Command*>       CommandList;
std::string                 CommandString;
Image*                      ConsoleFrame    = nullptr;
Image*                      DragFrame       = nullptr;  
Label*                      KeyInputLabel   = nullptr;

bool                        EnableConsoleFrameRate = false;

void SysPrint(std::string Text,int ErrorCode = 1)
{
    glm::vec3 Colour;
    if (Console.size() >= 16)   {Console.erase(Console.begin(),Console.begin()+1);}
    if      (ErrorCode == 1)    {Colour = CONSOLE_INFO;}
    else if (ErrorCode == 2)    {Colour = CONSOLE_WARN;}
    else if (ErrorCode == 3)    {Colour = CONSOLE_ERROR;}

    Label* NewTextLabel = new Label(Text,glm::vec2(0.0f,PositionY),1.0f,Colour);
    Console.push_back(NewTextLabel);
    for (int ConsoleIndex = 0; ConsoleIndex < Console.size(); ConsoleIndex++) 
    {Console[ConsoleIndex]->TextPosition = glm::vec2(0.0f,1050-(19.0f*ConsoleIndex));}
}

template <typename T> void SysPrintSafe(T Text,int ErrorCode = 1)
{
    std::string SafeString = std::to_string(Text);
    SysPrint(SafeString,ErrorCode);
}

void ToggleFrameRateTick(void)
{
    if (!EnableConsoleFrameRate)    {EnableConsoleFrameRate = true;}
    else                            {EnableConsoleFrameRate = false;}
}

void CommandAddCommand(std::string NewCommandString,Function CommandFunction)
{
    Command NewCommand;
    for (int CommandIndex = 0; CommandIndex > CommandList.size(); CommandIndex++)
    {
        if (CommandList[CommandIndex]->CommandString == NewCommandString) {SysPrint("Command already exits.",2); return;}
    }
    NewCommand.CommandString       = NewCommandString;
    NewCommand.ExecutableFunction  = CommandFunction;
    CommandList.push_back(&NewCommand);
}

void CommandGenerateBackFrame(int Width,int Height,unsigned char* ImageDataTop,unsigned char* ImageDataBottom,std::string FrameVertex,std::string FrameFragment)
{
    CommandAddCommand("ToggleFrameCounter",ToggleFrameRateTick);
    ConsoleFrame        = new Image(ImageDataTop,glm::vec2(0.0f,800.0f),glm::vec2(Width,600.0f),Width,Height,0.0f,FrameVertex,FrameFragment);
    DragFrame           = new Image(ImageDataBottom,glm::vec2(0.0f,-240.0f),glm::vec2(Width,1000.0f),Width,Height,0.0f,FrameVertex,FrameFragment);
    KeyInputLabel       = new Label("TextTest",glm::vec2(0.0f,0.0f),1.0f,CONSOLE_INFO);
}

void CommandUpdateInput(GLFWwindow* NewWindow)
{

    if (glfwGetKey(NewWindow,GLFW_KEY_TAB) == GLFW_PRESS)
    {
        CurrentOpenTime = glfwGetTime();
        if (CurrentOpenTime - LastOpenTime >= 1.5)
        {
            if (IsConsoleOpen)  {IsConsoleOpen = false;}
            else                {IsConsoleOpen = true;}
            LastOpenTime = CurrentOpenTime;
        }
    }
}

void CommandUpdateCommandExecution()
{
    int CommandIndex = -1;
    for (int Index = 0; Index < CommandList.size(); Index++)
    {
        if (CommandList[Index]->CommandString == CommandString) {CommandIndex = Index; break;}
    }
    if (CommandIndex != -1) {CommandList[CommandIndex]->ExecutableFunction();}
    else                    {SysPrint("Invalid/Unknown Command",3);}
}

void CommandUpdateKeyInput(GLFWwindow* NewWindow,unsigned int KeyCode)
{
    if (!IsConsoleOpen) {char NewCode = KeyCode; CommandString += NewCode;}
    KeyInputLabel->TextContent = CommandString;
}

void CommandMoveFrame()
{
    if (IsConsoleOpen)
    {
        ConsoleFrame->Position.y    += 8.0f;
        DragFrame->Position.y       -= 8.0f;
        if (ConsoleFrame->Position.y >= 1440)   {ConsoleFrame->Position.y   = 1440;}
        if (ConsoleFrame->Position.y <= 0)      {DragFrame->Position.y      = 0;}
        for (int ConsoleIndex = 0; ConsoleIndex < Console.size(); ConsoleIndex++)
        {if (!(ConsoleFrame->Position.y >= 1440)) {Console[ConsoleIndex]->TextPosition.y += 8.0f;KeyInputLabel->TextPosition.y = Console[ConsoleIndex]->TextPosition.y;}}
    }
    else
    {
        ConsoleFrame->Position.y    -= 8.0f;
        if (ConsoleFrame->Position.y <= 740)    {ConsoleFrame->Position.y   = 740;}
        for (int ConsoleIndex = 0; ConsoleIndex < Console.size(); ConsoleIndex++)
        {if (!(ConsoleFrame->Position.y <= 740)) {Console[ConsoleIndex]->TextPosition.y -= 8.0f;KeyInputLabel->TextPosition.y = Console[ConsoleIndex]->TextPosition.y;}}
    }
    for (int ConsoleIndex = 0; ConsoleIndex < Console.size(); ConsoleIndex++) {Console[ConsoleIndex]->RenderText();}
}

void CommandRenderText(GLFWwindow* NewWindow)
{
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    ConsoleFrame->RenderInstance();
    DragFrame->RenderInstance();

    if (glfwGetKey(NewWindow,GLFW_KEY_ENTER)        == GLFW_PRESS) {if (CommandString.size() != 0) {SysPrint(CommandString,1);CommandUpdateCommandExecution();CommandString = "";}}
    if (glfwGetKey(NewWindow,GLFW_KEY_BACKSLASH)    == GLFW_PRESS) {CurrentKeyTime = glfwGetTime(); if (CurrentKeyTime-LastKeyTime >= 2.0){CommandString.erase(CommandString.size()-1); LastKeyTime = CurrentKeyTime;}}

    glfwSetCharCallback(NewWindow,CommandUpdateKeyInput);
    CommandMoveFrame();
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void CommandDelete()
{
    for (int TextLabelIndex = 0; TextLabelIndex < Console.size(); TextLabelIndex++)
    {
        Console[TextLabelIndex]->DestroyText();
        if (Console[TextLabelIndex]) {delete Console[TextLabelIndex];} 
    }
    Console = {};
    KeyInputLabel->DestroyText();
    ConsoleFrame->DestroyInstance();
    DragFrame->DestroyInstance();
    if (ConsoleFrame)   delete ConsoleFrame;
    if (DragFrame)      delete DragFrame;
    if (KeyInputLabel)  delete KeyInputLabel;
}