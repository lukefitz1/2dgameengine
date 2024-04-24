#ifndef KEYBOARDCONTROLSYSTEM_H
#define KEYBOARDCONTROLSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Logger/Logger.h"

class KeyboardControlSystem: public System {
    public: 
        KeyboardControlSystem() {
            RequireComponent<KeyboardControlledComponent>();
            RequireComponent<SpriteComponent>();
            RequireComponent<RigidBodyComponent>();
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::onKeyPressed);
        }

        // This is the function that is going to be invoked if the event type CollisionEvent is emitted
        void onKeyPressed(KeyPressedEvent& event) {
            // TODO: change the sprite and the velocity of my entity
            for (auto entity: GetSystemEntities()) {
                const auto keyboardcontrol = entity.GetComponent<KeyboardControlledComponent>();
                auto& sprite = entity.GetComponent<SpriteComponent>();
                auto& rigidbody = entity.GetComponent<RigidBodyComponent>();

                switch (event.symbol) {
                    case SDLK_UP:
                        rigidbody.velocity = keyboardcontrol.upVelocity;
                        sprite.srcRect.y = sprite.height * 0;
                        break;
                    case SDLK_RIGHT:
                        rigidbody.velocity = keyboardcontrol.rightVelocity;
                        sprite.srcRect.y = sprite.height * 1;
                        break;
                    case SDLK_DOWN:
                        rigidbody.velocity = keyboardcontrol.downVelocity;
                        sprite.srcRect.y = sprite.height * 2;
                        break;
                    case SDLK_LEFT:
                        rigidbody.velocity = keyboardcontrol.leftVelocity;
                        sprite.srcRect.y = sprite.height * 3;
                        break;
                }
            }
        }

        void Update() {}
};

#endif