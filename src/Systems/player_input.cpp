#include "player_input.hpp"

#include "../Components/player.hpp"

class inputHandler{
    enum State{
        PRESSED,
        RELEASED
    };
    State inputState[];

    void handleInputEvent(SDL_Scancode scancode){
        if(inputState[scancode] == PRESSED){
            inputState[scancode] = RELEASED;
        }
        inputState[scancode] = PRESSED;
    }

    bool isPressed(SDL_Scancode scancode){
        return (inputState[scancode] == PRESSED);
    }

    bool isReleased(SDL_Scancode scancode){
        return (inputState[scancode] == RELEASED);
    }
};

void playerInput(entt::registry &reg, SDL_Scancode scancode){
    inputHandler.handleInputEvent(scancode);
    // const auto view = reg.view<Player>();
    // for(const entt::entity e: view) {
    //     auto &player = view.get<Player>(e);        

    //     player.lastAction = player.currentAction;
    //     switch(scancode){
    //         case SDL_SCANCODE_D:
    //         case SDL_SCANCODE_RIGHT:
    //             player.currentAction = WALK_RIGHT;
    //             player.direction     = LOOKING_RIGHT;
    //             break;
    //         case SDL_SCANCODE_A:
    //         case SDL_SCANCODE_LEFT:
    //             player.currentAction = WALK_LEFT;
    //             player.direction     = LOOKING_LEFT;
    //             break;
    //         case SDL_SCANCODE_SPACE:
    //             player.currentAction = JUMP;
    //             break;
    //         case SDL_SCANCODE_LSHIFT:
    //             player.currentAction = RUN_LEFT;
    //             player.direction     = LOOKING_LEFT;
    //             break;
    //         case SDL_SCANCODE_E:
    //             player.currentAction = ATTACK;
    //             break;
    //         case SDL_SCANCODE_R:
    //             player.currentAction = RESET;
    //             break;
    //         default:
    //             player.currentAction = IDLE;
    //             break;
    //     }
    // }
}

void playerDefault(entt::registry &reg, SDL_Scancode scancode){
    inputHandler.handleInput(scancode);
    // const auto view = reg.view<Player>();
    // for(const entt::entity e: view) {
    //     auto &player = view.get<Player>(e);        

    //     player.lastAction = player.currentAction;
    //     player.currentAction = IDLE;
    // }
}