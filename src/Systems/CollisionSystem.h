#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"

class CollisionSystem: public System {
    public:
        CollisionSystem() {
            RequireComponent<TransformComponent>();
            RequireComponent<BoxColliderComponent>();
        }

        void Update() {
            // Check all entities that have a box collider to see if they are colliding with each other
            // Tip: AABB collision check - Axis-Aligned Bounding Boxes
            auto entities = GetSystemEntities();

            // loop all the entities that the system is interested in
            for (auto i = entities.begin(); i != entities.end(); i++) {
                Entity a = *i;
                auto aTransform = a.GetComponent<TransformComponent>();
                auto aCollider = a.GetComponent<BoxColliderComponent>();

                // loop all the entities that still need to be checked (to the "right" of i)
                for (auto j = i; j != entities.end(); j++) {
                    Entity b = *j;

                    // Bypass if we are trying to test the same entity
                    if (a == b) {
                        continue;
                    }

                    auto bTransform = b.GetComponent<TransformComponent>();
                    auto bCollider = b.GetComponent<BoxColliderComponent>();

                    // TODO: Check for collision between a & b (AABB)
                    bool collisionHappened = CheckAABBCollision(
                        aTransform.position.x + aCollider.offset.x,
                        aTransform.position.y + aCollider.offset.y,
                        aCollider.width,
                        aCollider.height,
                        bTransform.position.x + bCollider.offset.x,
                        bTransform.position.y + bCollider.offset.y,
                        bCollider.width,
                        bCollider.height
                    );

                    if (collisionHappened) {
                        Logger::Log("Entity " + std::to_string(a.GetId()) + " is colliding with entity " + std::to_string(b.GetId()));

                        // TODO: emit an event
                    }
                }
            }
        }

        bool CheckAABBCollision(double aX, double aY, double aW, double aH, double bX, double bY, double bW, double bH) {
            return (
                aX < bX + bW &&
                aX + aW > bX && 
                aY < bY + bH && 
                aY + aH > bY
            );
        }
};

#endif