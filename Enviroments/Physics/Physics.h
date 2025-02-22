#ifndef __PHYSICS_HEADER__
#define __PHYSICS_HEADER__

#include <map>
#include "Colliders/Collision.h"

class Physics
{
    public:
        Physics();
        glm::vec3   RenderEntityGravity(int EntityIndex,float DeltaTime);
        void        SetEntityVelocity(int EntityIndex);
    private:
        std::map<int,float> VelocityTable;
};

#endif