#include "game.hpp"

#include <SDL_image.h>
#include <math.h>
#include <vector>

#include "../Core/factories.hpp"
#include "../Core/render.hpp"
#include "../Core/load.hpp"
#include "../Util/sdl_check.hpp"
#include "../Components/components.hpp"
#include "../Systems/player_input.hpp"
#include "../Systems/player_movement.hpp"
#include "../Systems/change_levels.hpp"
#include "../Systems/player_initial_position.hpp"
#include "../Systems/toggle_dev_info.hpp"
#include "../Systems/set_camera.hpp"

GameSettings game;
Camera       camera;

entt::registry reg;
entt::entity player;

std::vector<entt::entity> levels;

int currentLevel = 3;
bool showDevInfo = false;

GameSettings gameInit(){
    game.WIDTH           = 1500;
    game.HEIGHT          = 800;
    game.FRAMES          = 0;
    game.FPS_CAP         = 60;
    game.TICKS_PER_FRAME = 1000 / game.FPS_CAP;
    game.QUIT            = false;
    game.FONT            = NULL;
    game.CURRENT_FPS     = 0.0;
    game.AVERAGE_FPS     = 0.0;
    game.CURRENT_PERF    = 0.0;

    camera.position.x = 0;
    camera.position.y = 0;
    camera.size.width = game.WIDTH;
    camera.size.height = game.HEIGHT;

    // camera.position.x -= 760;

    SDL_CHECK(loadFramerateFont(&game));

    return game;
}

void gameCreateEntities(SDL_Renderer* renderer){
    player = makePlayer(renderer, reg, game);

    Color color;
    color.red   = 37;
    color.green = 112;
    color.blue  = 78;    
    color.alpha = 255;
    levels.push_back(makeLevel(renderer, reg, game, 0.8, 0.8, color, "Level One"));

    color.green = 78; 
    color.blue  = 112;
    color.alpha = 255;
    levels.push_back(makeLevel(renderer, reg, game, 0.6, 0.7, color, "Level Two"));

    color.red   = 255;
    color.green = 71; 
    color.blue  = 71;
    color.alpha = 255;
    levels.push_back(makeLevel(renderer, reg, game, 0.4, 0.8, color, "Level Three"));

    color.red   = 255;
    color.green = 238; 
    color.blue  = 11;
    color.alpha = 255;
    levels.push_back(makeLevel(renderer, reg, game, 5, 0.28, color, "Level Four"));

    color.red   = 0;
    color.green = 39; 
    color.blue  = 43;
    color.alpha = 255;
    levels.push_back(makeLevel(renderer, reg, game, 1, 1, color, "Level Five"));
    
    playerInitialPosition(reg, levels[currentLevel], player);
}

void gameInput(SDL_Scancode scancode, const Uint8* currentKeyStates){
    playerInput(reg, scancode, currentKeyStates);
}

void gameImmediateInput(SDL_Scancode scancode){
    toggleDevInfo(scancode, showDevInfo);
    changeLevels(reg, scancode, player, currentLevel, levels);
}

void gameDefaultInput(SDL_Scancode scancode){    
    playerDefault(reg, scancode);
}

void gameLogic(double t, float dt){    
    playerMovement(reg, dt, levels[currentLevel]);
}

void gameRender(SDL_Renderer* renderer){
    setCamera(renderer, reg, game, camera, player, levels[currentLevel]);
    // renderFrameRate(renderer, game);
    renderLevel(renderer, reg, game, camera, levels[currentLevel]);
    renderPlayer(renderer, reg, &game, camera, showDevInfo);
    renderLevelInfo(renderer, reg, game, levels[currentLevel]);
    renderPlayerInfo(renderer, reg, game);
}

void gameQuit(SDL_Window* window, SDL_Renderer* renderer){
    const auto view = reg.view<IdleSprite, RunSprite>();
    for(const entt::entity e : view){
        auto &idleSprite = view.get<IdleSprite>(e);
        auto &runSprite = view.get<RunSprite>(e);

        SDL_DestroyTexture(idleSprite.textureSheet);
        SDL_DestroyTexture(runSprite.textureSheet);
        idleSprite.textureSheet = NULL;
        runSprite.textureSheet  = NULL;
    }

    game.FONT = NULL;

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    window = NULL;
    renderer = NULL;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}