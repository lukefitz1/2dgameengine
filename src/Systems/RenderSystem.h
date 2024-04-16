#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../AssetStore/AssetStore.h"
#include <SDL2/SDL.h>

class RenderSystem: public System {
    public: 
        RenderSystem() {
            RequireComponent<TransformComponent>();
            RequireComponent<SpriteComponent>();
        };

        void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore) {
            // Create a vector with both Sprie and Transform component of all entities
            struct RenderableEntity {
                TransformComponent transfromComponent;
                SpriteComponent spriteComponent;
            };
            std::vector<RenderableEntity> renderableEntities;
            for (auto entity: GetSystemEntities()) {
                RenderableEntity renderableEntity;
                renderableEntity.spriteComponent = entity.GetComponent<SpriteComponent>();
                renderableEntity.transfromComponent = entity.GetComponent<TransformComponent>();
                
                renderableEntities.emplace_back(renderableEntity);
            }

            // Sort the vector by the z-index value
            std::sort(renderableEntities.begin(), renderableEntities.end(), [](const RenderableEntity& a, const RenderableEntity& b) {
                return a.spriteComponent.zIndex < b.spriteComponent.zIndex;
            });

            // Loop all entities that the system is interested in
            for (auto entity: renderableEntities) {
                // Update entity position based on its velocity
                const auto transform = entity.transfromComponent;
                const auto sprite = entity.spriteComponent;

                // Set the source rectangle of our original sprite texture
                SDL_Rect srcRect = sprite.srcRect;

                // Set the destination rectangle with the x,y position to be rendered
                SDL_Rect dstRec = {
                    static_cast<int>(transform.position.x),
                    static_cast<int>(transform.position.y),
                    static_cast<int>(sprite.width * transform.scale.x), 
                    static_cast<int>(sprite.height * transform.scale.y)
                };

                SDL_RenderCopyEx(
                    renderer, 
                    assetStore->GetTexture(sprite.assetId),
                    &srcRect, 
                    &dstRec,
                    transform.rotation,
                    NULL,
                    SDL_FLIP_NONE);

                // TODO Draw PNG textures
            }
        };
};

#endif