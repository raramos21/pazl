#include "game.hpp"

#include "factories.hpp"
#include "render.hpp"

#include "../Util/sdl_check.hpp"
#include "../Components/player.hpp"
#include "../Systems/player_input.hpp"
#include "../Systems/player_movement.hpp"

#include <SDL_image.h>
#include <math.h>

void gameInit(SDL_Renderer* renderer, entt::registry &reg, GameSettings* game) {
    const entt::entity player = makePlayer(renderer, reg, game);
}

void gameInput(entt::registry& reg, SDL_Scancode scancode) {    
    playerInput(reg, scancode);
}

void gameDefaultInput(entt::registry& reg) {    
    playerDefault(reg);
}

void gameLogic(entt::registry& reg, double t, float dt) {    
    playerMovement(reg, dt);
}

void gameRender(SDL_Renderer* renderer, entt::registry &reg, GameSettings* game) {
    // renderFrameRate(renderer, game);
    renderPlayer(renderer, reg, game);
    renderPlayerInfo(renderer, reg, game);
}

void gameQuit(SDL_Window* window, SDL_Renderer* renderer, entt::registry &reg, GameSettings* game) {
    const auto view = reg.view<IdleSprite, RunSprite>();
    for(const entt::entity e : view){
        auto &idleSprite = view.get<IdleSprite>(e);
        auto &runSprite = view.get<RunSprite>(e);

        SDL_DestroyTexture(idleSprite.textureSheet);
        SDL_DestroyTexture(runSprite.textureSheet);
        idleSprite.textureSheet = NULL;
        runSprite.textureSheet  = NULL;
    }

    game->FONT = NULL;

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    window = NULL;
    renderer = NULL;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}