#ifndef __WORLD_HEADER__
#define __WORLD_HEADER__

#include <map>
#include "Instance.h"
#include "../Player.h"
#include "../Physics/Colliders/CollisionShape.h"
#include "../Enviroments/Rendering/Scene/Scene.h"
#include "../Rendering/Terrain/Water.h"
#include "../Rendering/Terrain/Terrain.h"

class World
{
    public:
        World();
        void GenerateWorld(Parser* WorldParser);
        void RenderWorld(Camera *EngineCamera,float Near,float Far,float FieldOfView,unsigned int ReflectanceTexture);
        void DestroyWorld();
        int                                         LevelIndex = 0;
        Scene*                                      NewLevelScene;
        glm::mat4                                   TranslationMatrix;
        glm::mat4                                   RotationMatrix;
        glm::mat4                                   CollisionRotationMatrix;
        glm::mat4                                   ScaleMatrix;
    private:
        std::vector<float>                          GetSceneVertices();
        std::vector<unsigned int>                   GetSceneIndices();
        Collisions*                                 SceneCollider;
        std::map<int,std::vector<float>>            VertexCache;
        std::map<int,std::vector<unsigned int>>     IndicesCache;

        glm::mat4                                   PlayerTranslation;
        glm::mat4                                   PlayerRotation;
        glm::mat4                                   PlayerScale;
        
        Player*                                     PlayerWorld;
        CollisionShape*                             WorldCollisionShape;
        Water*                                      WorldWater;
        Terrain*                                    WorldTerrain;
        Shader*                                     CollisionShader;
        Shader*                                     SceneShaderInstance;
        Instance*                                   NewInstance;
        nlohmann::json                              NewJson;
        Parser*                                     CurrentParser;
};

#endif