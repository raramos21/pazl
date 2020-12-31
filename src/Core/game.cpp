#include "game.hpp"

#include "factories.hpp"
#include "render.hpp"

#include "../Util/sdl_check.hpp"
#include "../Components/player.hpp"

#include <SDL_image.h>
#include <math.h>


void gameInit(SDL_Renderer* renderer, entt::registry &reg, GameSettings* game) {
    const entt::entity player = makePlayer(renderer, reg, game);
}

void gameInput(entt::registry& reg, SDL_Scancode key) {    
    Movement movement; 
    switch(key){
        case SDL_SCANCODE_D:
            movement = RIGHT;
            break;
        case SDL_SCANCODE_A:
            movement = LEFT;
            break;
        case SDL_SCANCODE_SPACE:
            movement = JUMP;
            break;
        case SDL_SCANCODE_LSHIFT:
            movement = RUN;
            break;
        default:
            break;
    }

    const auto view = reg.view<Player, Position>();
    for(const entt::entity e: view) {
        auto &position = view.get<Position>(e);

        switch(movement){
            case RIGHT:
                position.x += 20.0f;
                break;
            case LEFT:
                position.x -= 20.0f;
                break;
            case JUMP:
                position.y += 10.0f;
                break;
            case RUN:
                position.y += 40.0f;
                break;
            default:
                break;
        }
    }
}

void gameLogic() {

}

void gameRender(SDL_Renderer* renderer, entt::registry &reg, GameSettings* game) {
    renderFrameRate(renderer, game);
    renderPlayer(renderer, reg, game);
}

void gameQuit(SDL_Window* window, SDL_Renderer* renderer, entt::registry &reg, GameSettings* game) {
    printf("QUIT!!!\n");

    const auto view = reg.view<IdleAnimationSprite>();
    for(const entt::entity e : view){
        IdleAnimationSprite &idleSprite = view.get<IdleAnimationSprite>(e);
        SDL_DestroyTexture(idleSprite.textureSheet);
        idleSprite.textureSheet = NULL;
    }

    game->gFont = NULL;

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    window = NULL;
    renderer = NULL;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}