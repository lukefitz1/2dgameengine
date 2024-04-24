#ifndef KEYBOARDCONTROLSYSTEM_H
#define KEYBOARDCONTROLSYSTEM_H

#include "../ECS/ECS.h"
// #include "../Components/BoxColliderComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Logger/Logger.h"

class KeyboardControlSystem: public System {
    public: 
        KeyboardControlSystem() {}

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::onKeyPressed);
        }

        // This is the function that is going to be invoked if the event type CollisionEvent is emitted
        void onKeyPressed(KeyPressedEvent& event) {
            std::string keyCode = std::to_string(event.symbol);
            std::string keySymbol(1, event.symbol);
            Logger::Log("Key pressed event emitted: [" + keyCode + "] " + keySymbol);
        }

        void Update() {}
};

#endif