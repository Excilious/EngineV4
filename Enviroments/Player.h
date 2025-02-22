#ifndef __PLAYER_HEADER__
#define __PLAYER_HEADER__

#include "Rendering/Camera.h"
#include "Physics/Physics.h"

class Player: public Camera
{
    public:
        Player(int WindowWidth,int WindowHeight,glm::vec3 PlayerDefaultPosition);
        void CheckPlayerMovementInputs(GLFWwindow* InputWindow,std::vector<float> SceneVertices,std::vector<unsigned int> SceneIndices,glm::mat4 SceneMatrix);
        void Destroy();
    private:
        Collisions* PlayerCollisionStatus;
};

#endif