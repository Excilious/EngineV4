#ifndef __CONSOLE_H__
#define __CONSOLE_H__
#define CONSOLE_ERROR   glm::vec3(1.0f,0.0f,0.0f)
#define CONSOLE_WARN    glm::vec3(1.0f,1.0f,0.0f)
#define CONSOLE_INFO    glm::vec3(1.0f,1.0f,1.0f)

#include <windows.h>
#include "../Rendering/Interface/Label.h"
#include "../Rendering/Interface/Image.h"
#include "../Dependancies/GLFW/glfw3.h"

typedef void (*Function) (void);
void CommandAddCommand(std::string NewCommandString,Function CommandFunction);
void CommandUpdateCommandExecution();
void UpdateTextLayout();

void ToggleFrameRateTick(void);

extern bool EnableConsoleFrameRate;
extern bool IsConsoleOpen;
extern std::vector<Label*> Console;
extern void SysPrint(std::string Text,int ErrorCode);
extern void CommandUpdateInput(GLFWwindow* NewWindow);
extern void CommandRenderText(GLFWwindow* NewWindow);
extern void CommandGenerateBackFrame(int Width,int Height,unsigned char* ImageDataTop,unsigned char* ImageDataBottom,std::string FrameVertex,std::string FrameFragment);
extern void CommandUpdateKeyInput(GLFWwindow* NewWindow,unsigned int KeyCode);
extern void CommandDelete();

#endif