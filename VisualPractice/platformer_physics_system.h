#ifndef TE_PLATFORMER_PHYSICS_H
#define TE_PLATFORMER_PHYSICS_H

#include <memory>
#include <glm/glm.hpp>

namespace te
{
    class PhysicsComponent;
    class TransformComponent;
    class BoundingBoxComponent;
    class TiledMap;

    class PlatformerPhysicsSystem
    {
    public:
        PlatformerPhysicsSystem(
            std::shared_ptr<PhysicsComponent> pPhysics,
            std::shared_ptr<TransformComponent> pTransform,
            std::shared_ptr<BoundingBoxComponent> pBoundingBox,
            std::shared_ptr<TiledMap> pTiledMap,
            float gravityAcceleration);

        void update(float dt) const;

    private:
        std::shared_ptr<PhysicsComponent> mpPhysics;
        std::shared_ptr<TransformComponent> mpTransform;
        std::shared_ptr<BoundingBoxComponent> mpBoundingBox;
        std::shared_ptr<TiledMap> mpTiledMap;
        glm::vec2 mGravityAcceleration;
    };
}

#endif /* TE_PLATFORMER_PHYSICS_H */