#include "player_collide_with_platforms.hpp"

#include "../Components/components.hpp"

void playerCollideWithPlatforms(entt::registry &reg, float dt, entt::entity playerEntity, entt::entity levelEntity){
    auto &player = reg.get<Player>(playerEntity);
    auto &playerPosition = reg.get<Position>(playerEntity);
    auto &playerForce = reg.get<Force>(playerEntity);
    auto &playerSize = reg.get<IdleSprite>(playerEntity);

    const auto view = reg.view<Platform, Size, Position, Color>();
    for(const entt::entity e : view){
        const auto platform = view.get<Platform>(e);       

        if(platform.level == levelEntity){
            const auto platformPosition = view.get<Position>(e);
            const auto platformSize     = view.get<Size>(e);

            if(playerPosition.y <= (platformPosition.y + platformSize.height) 
                && (playerPosition.x + playerSize.size.width) >= platformPosition.x 
                && playerPosition.x <= (platformPosition.x + platformSize.width)){
                player.isFalling = true;
                playerPosition.y = (platformPosition.y + platformSize.height);
                playerForce.y = 0.0f;
            } 
            
            // else if((playerPosition.x + playerSize.size.width) >= platformPosition.x){
            //     // playerPosition.x = platformPosition.x;
            //     playerForce.x = 0.0f;
            // }

        }
        
    }
}