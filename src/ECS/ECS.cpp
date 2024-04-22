#include "ECS.h"
#include "../Logger/Logger.h"

int IComponent::nextId = 0;

int Entity::GetId() const {
    return id;
}

void Entity::Kill() {
    registry->KillEntity(*this);
}

void System::AddEntityToSystem(Entity entity) {
    entities.push_back(entity);
};

void System::RemoveEntityToSystem(Entity entity) {
    entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](Entity other) {
        return entity == other;
    }), entities.end());
};

std::vector<Entity> System::GetSystemEntities() const {
    return entities;
};

const Signature& System::GetComponentSignature() const {
    return componentSignature;
};

Entity Registry::CreateEntity() {
    int entityId;

    if (freeIds.empty()) {
        // if there are no free ids waiting to be reused, created new one
        entityId = numEntities++;
        // Make sure the entityComponentSignatures vector can accomodate the new entity
        if (entityId >= entityComponentSignatures.size()) {
            entityComponentSignatures.resize(entityId + 1);
        }
    } else {
        // Reuse an id from the list of previously removed entities
        entityId = freeIds.front();
        freeIds.pop_front();
    }

    Entity entity(entityId);
    entity.registry = this;
    entitiesToBeAdded.insert(entity);

    Logger::Log("Entity created with id = " + std::to_string(entityId));
    return entity;
}

void Registry::KillEntity(Entity entity) {
    entitiesToBeKilled.insert(entity);
}

void Registry::AddEntityToSystem(Entity entity) {
    const auto entityId = entity.GetId();

    // TODO: match entityComponentSignature <---> systemComponentSignature
    const auto& entityComponentSignature = entityComponentSignatures[entityId];

    // Loop all the systems
    for (auto& system: systems) {
        const auto& systemComponentSignature = system.second->GetComponentSignature();

        bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;
        if (isInterested) {
            // TODO: Add entity to the system
            system.second->AddEntityToSystem(entity);
        }
    }
}

// What does `second` do in this method?
void Registry::RemoveEntityFromSystems(Entity entity) {
    for (auto system: systems) {
        system.second->RemoveEntityToSystem(entity);
    }
}

void Registry::Update() {
    // Processing the entities that are waiting to be created to the active systems
    for (auto entity: entitiesToBeAdded) {
        AddEntityToSystem(entity);
    }
    entitiesToBeAdded.clear();

    // Processing the entities that are waiting to be kill to the active systems
    for (auto entity: entitiesToBeKilled) {
        RemoveEntityFromSystems(entity);
        entityComponentSignatures[entity.GetId()].reset();

        // Make the entity id available to be reused
        freeIds.push_back(entity.GetId());
    }
    entitiesToBeKilled.clear();
}