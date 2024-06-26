#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include "../Logger/Logger.h"

class DamageSystem: public System {
    public: 
        DamageSystem() {
            RequireComponent<BoxColliderComponent>();
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::onCollision);
        }

        // This is the function that is going to be invoked if the event type CollisionEvent is emitted
        void onCollision(CollisionEvent& event) {
            Logger::Log("The Damage System received an event collision between entities " + std::to_string(event.a.GetId()) + " and " + std::to_string(event.b.GetId()));
            // event.a.Kill();
            // event.b.Kill();
        }

        void Update() {

        }
};

#endif
