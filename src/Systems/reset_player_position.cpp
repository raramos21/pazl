#include "reset_player_position.hpp"

#include "../Components/components.hpp"
#include "../Systems/player_initial_position.hpp"

void resetPlayerPosition(entt::registry & reg, SDL_Scancode scancode, entt::entity playerEntity, entt::entity levelEntity){
    if(scancode == SDL_SCANCODE_R){
        playerInitialPosition(reg, levelEntity, playerEntity);
    }
}