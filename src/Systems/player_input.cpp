#include "player_input.hpp"

#include "../Components/player.hpp"

void playerInput(entt::registry &reg, SDL_Scancode scancode, const Uint8* currentKeyStates){
    const auto view = reg.view<Player>();
    for(const entt::entity e: view) {
        auto &player = view.get<Player>(e);        

        player.lastAction = player.currentAction;
        if(currentKeyStates[SDL_SCANCODE_D] || currentKeyStates[SDL_SCANCODE_RIGHT]){
            player.currentAction = WALK_RIGHT;
            player.direction     = LOOKING_RIGHT;
        }

        if(currentKeyStates[SDL_SCANCODE_A] || currentKeyStates[SDL_SCANCODE_LEFT]){
            player.currentAction = WALK_LEFT;
            player.direction     = LOOKING_LEFT;
        }

        if(currentKeyStates[SDL_SCANCODE_SPACE]){
            player.currentAction = JUMP;
        }

        if(currentKeyStates[SDL_SCANCODE_LSHIFT] && (currentKeyStates[SDL_SCANCODE_LEFT] || currentKeyStates[SDL_SCANCODE_A])){
            player.currentAction = RUN_LEFT;
            player.direction     = LOOKING_LEFT;
        }

        if(currentKeyStates[SDL_SCANCODE_LSHIFT] && (currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_D])){
            player.currentAction = RUN_RIGHT;
            player.direction     = LOOKING_RIGHT;
        }

        if(currentKeyStates[SDL_SCANCODE_E]){
            player.currentAction = ATTACK;
        }

        if(currentKeyStates[SDL_SCANCODE_R]){
            player.currentAction = RESET;
        }
    }
}

void playerDefault(entt::registry &reg, SDL_Scancode scancode){
    const auto view = reg.view<Player>();
    for(const entt::entity e: view) {
        auto &player = view.get<Player>(e);        

        player.lastAction = player.currentAction;
        player.currentAction = IDLE;
    }
}