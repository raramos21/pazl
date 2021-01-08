#include "game.hpp"

#include "factories.hpp"
#include "render.hpp"

#include "../Util/sdl_check.hpp"
#include "../Components/components.hpp"
#include "../Systems/player_input.hpp"
#include "../Systems/player_movement.hpp"

#include <SDL_image.h>
#include <math.h>
#include <vector>

int currentLevel = 0;
std::vector<entt::entity> levels;

void gameInit(SDL_Renderer* renderer, entt::registry &reg, GameSettings* game){
    const entt::entity player = makePlayer(renderer, reg, game);
    Color color;
    color.red   = 37;
    color.green = 112;
    color.blue  = 78;    
    color.alpha = 255;
    levels.push_back(makeLevel(renderer, reg, game, 0.8, 0.8, color, "Level One"));

    color.green = 78; 
    color.blue  = 112;
    levels.push_back(makeLevel(renderer, reg, game, 0.6, 0.7, color, "Level Two"));

    color.red   = 255;
    color.green = 71; 
    color.blue  = 71;
    levels.push_back(makeLevel(renderer, reg, game, 0.4, 0.8, color, "Level Three"));

    color.red   = 255;
    color.green = 238; 
    color.blue  = 11;
    levels.push_back(makeLevel(renderer, reg, game, 0.95, 0.3, color, "Level Four"));

    color.red   = 0;
    color.green = 39; 
    color.blue  = 43;
    levels.push_back(makeLevel(renderer, reg, game, 1, 1, color, "Level Five"));
}

void gameInput(entt::registry& reg, SDL_Scancode scancode, const Uint8* currentKeyStates){
    playerInput(reg, scancode, currentKeyStates);
}

void gameChangeLevels(SDL_Scancode scancode){
    switch(scancode){
        case SDL_SCANCODE_COMMA:
            currentLevel -= 1;
            if(currentLevel < 0 ){
                currentLevel = 0;
            }
            break;
        case SDL_SCANCODE_PERIOD:
            currentLevel += 1;        
            if(currentLevel >= levels.size()-1){
                currentLevel = levels.size()-1;
            }
            break;
        default:
            break;
    }  
}

void gameDefaultInput(entt::registry& reg, SDL_Scancode scancode){    
    playerDefault(reg, scancode);
}

void gameLogic(entt::registry& reg, double t, float dt){    
    playerMovement(reg, dt, levels[currentLevel]);
}

void gameRender(SDL_Renderer* renderer, entt::registry &reg, GameSettings* game){
    // renderFrameRate(renderer, game);
    renderLevel(renderer, reg, game, levels[currentLevel]);
    renderPlayer(renderer, reg, game);
    renderLevelInfo(renderer, reg, game, levels[currentLevel]);
    renderPlayerInfo(renderer, reg, game);
}

void gameQuit(SDL_Window* window, SDL_Renderer* renderer, entt::registry &reg, GameSettings* game){
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