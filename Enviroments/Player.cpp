#include "Player.h"
#include "Common/Console.h" 

Player::Player(int WindowWidth,int WindowHeight,glm::vec3 PlayerDefaultPosition) : Camera(WindowWidth,WindowHeight,PlayerDefaultPosition)
{
    this->PlayerCollisionStatus = new Collisions();
    this->Position              = PlayerDefaultPosition;

}

void Player::CheckPlayerMovementInputs(GLFWwindow* InputWindow,std::vector<float> SceneVertices,std::vector<unsigned int> SceneIndices,glm::mat4 SceneMatrix)
{
    glfwSetInputMode(InputWindow,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    if (IsConsoleOpen)
    {
        glm::vec3 OldPosition = this->Position;
        if (this->PlayerCollisionStatus->IntersectPointWithTriangle(this->Position,SceneVertices,SceneIndices,SceneMatrix,0.001f,3))
        {SysPrint("Dude",2);}
        if (glfwGetKey(InputWindow,GLFW_KEY_W) == GLFW_PRESS) {this->Position += this->Speed * this->Orientation;}
        if (glfwGetKey(InputWindow,GLFW_KEY_A) == GLFW_PRESS) {this->Position += this->Speed * -glm::normalize(glm::cross(this->Orientation,this->UpPosition));}
        if (glfwGetKey(InputWindow,GLFW_KEY_S) == GLFW_PRESS) {this->Position += this->Speed * -this->Orientation;}
        if (glfwGetKey(InputWindow,GLFW_KEY_D) == GLFW_PRESS) {this->Position += this->Speed * glm::normalize(glm::cross(this->Orientation,this->UpPosition));}
    }
}

void Player::Destroy()
{
    if (this->PlayerCollisionStatus) {delete this->PlayerCollisionStatus;}
}