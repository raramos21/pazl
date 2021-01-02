#include "player_input.hpp"
#include "../Components/player.hpp"

void playerInput(entt::registry &reg, SDL_Scancode scancode){
    const auto view = reg.view<Player>();
    for(const entt::entity e: view) {
        auto &player = view.get<Player>(e);        

        player.lastAction = player.currentAction;
        switch(scancode){
            case SDL_SCANCODE_D:
                player.currentAction =  MOVE_RIGHT;
                break;
            case SDL_SCANCODE_A:
                player.currentAction =  MOVE_LEFT;
                break;
            case SDL_SCANCODE_SPACE:
                player.currentAction =  JUMP;
                break;
            case SDL_SCANCODE_LSHIFT:
                player.currentAction =  RUN;
                break;
            case SDL_SCANCODE_E:
                player.currentAction = ATTACK;
                break;
            default:
                player.currentAction =  IDLE;
                break;
        }
    }
}

void playerDefault(entt::registry &reg){
    const auto view = reg.view<Player>();
    for(const entt::entity e: view) {
        auto &player = view.get<Player>(e);        

        player.lastAction = player.currentAction;
        player.currentAction = IDLE;
    }
}