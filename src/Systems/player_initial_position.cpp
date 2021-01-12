#include "player_initial_position.hpp"

#include "../Components/components.hpp"

void playerInitialPosition(entt::registry & reg, entt::entity levelEntity, entt::entity playerEntity){
    const auto level         = reg.get<Level>(levelEntity);
    const auto levelPosition = reg.get<Position>(levelEntity);
    const auto sprite        = reg.get<IdleSprite>(playerEntity);
    auto &playerPosition     = reg.get<Position>(playerEntity);
    auto &player             = reg.get<Player>(playerEntity);

    playerPosition.y = level.floor - sprite.size.height;
    playerPosition.x = levelPosition.x;
    player.isGoingToCollide = false;
    player.isColliding = false;
}