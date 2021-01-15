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
#include "../Systems/player_initial_position.hpp"
#include "../Systems/change_levels.hpp"
#include "../Systems/toggle_dev_info.hpp"
#include "../Systems/camera_position.hpp"
#include "../Systems/camera_initial_position.hpp"
#include "../Systems/player_collide_with_platforms.hpp"
#include "../Systems/reset_player_position.hpp"

GameSettings game;
Camera       camera;

entt::registry reg;
entt::entity player;

std::vector<entt::entity> levels;

int currentLevel = 0;
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
    entt::entity levelOne = makeLevel(reg, game, 0.8, 0.8, color, "Level One");
    
    Position position{(float) (game.WIDTH/2), (float) (game.HEIGHT/2 + 280)};
    Size size{100, 30};

    color.red   = 0;
    color.green = 0;
    color.blue  = 0;

    auto platformEntity1     = makePlatform(reg, game, levelOne, position, size, color);
    auto collisionBoxEntity1 = makeCollisionBox(reg, levelOne, position, size, action::NONE, direction::NONE);

    auto &platform1        = reg.get<Platform>(platformEntity1);
    platform1.collisionBox = collisionBoxEntity1;

    // position = {(float) (game.WIDTH/2) + 100, (float) (game.HEIGHT/2 + 240)};

    // auto platformEntity2     = makePlatform(reg, game, levelOne, position, size, color);
    // auto collisionBoxEntity2 = makeCollisionBox(reg, levelOne, position, size, action::NONE, direction::NONE);

    // auto &platform2        = reg.get<Platform>(platformEntity2);
    // platform2.collisionBox = collisionBoxEntity2;

    // position = {(float) (game.WIDTH/2) + 200, (float) (game.HEIGHT/2 + 200)};

    // auto platformEntity3     = makePlatform(reg, game, levelOne, position, size, color);
    // auto collisionBoxEntity3 = makeCollisionBox(reg, levelOne, position, size, action::NONE, direction::NONE);

    // auto &platform3        = reg.get<Platform>(platformEntity3);
    // platform3.collisionBox = collisionBoxEntity3;

    levels.push_back(levelOne);

    color.green = 78; 
    color.blue  = 112;
    color.alpha = 255;
    levels.push_back(makeLevel(reg, game, 0.6, 0.7, color, "Level Two"));

    color.red   = 255;
    color.green = 71; 
    color.blue  = 71;
    color.alpha = 255;
    levels.push_back(makeLevel(reg, game, 0.4, 0.8, color, "Level Three"));

    color.red   = 255;
    color.green = 238; 
    color.blue  = 11;
    color.alpha = 255;
    entt::entity levelFour = makeLevel(reg, game, 5, 0.28, color, "Level Four");
    levels.push_back(levelFour);

    color.red   = 255;
    color.green = 0;
    color.blue  = 0;
    position = {(float) (game.WIDTH/2), (float) (game.HEIGHT/2)};
    makePlatform(reg, game, levelFour, position, size, color);
    makeCollisionBox(reg, levelFour, position, size, action::NONE, direction::NONE);


    color.red   = 0;
    color.green = 39; 
    color.blue  = 43;
    color.alpha = 255;
    levels.push_back(makeLevel(reg, game, 1.10, 1, color, "Level Five"));
    
    playerInitialPosition(reg, levels[currentLevel], player);
    cameraInitialPosition(reg, game, camera, player, levels[currentLevel]);
}

void gameInput(SDL_Scancode scancode, const Uint8* currentKeyStates){
    playerInput(reg, scancode, currentKeyStates);
}

void gameImmediateInput(SDL_Scancode scancode){
    // I -key
    toggleDevInfo(scancode, showDevInfo);
    // </> -keys
    changeLevels(reg, scancode, game, camera, player, currentLevel, levels);
    // R -key
    resetPlayerPosition(reg, scancode, player, levels[currentLevel]);
}

void gameDefaultInput(SDL_Scancode scancode){    
    playerDefault(reg, scancode);
}

void gameLogic(double t, float dt){    
    playerMovement(reg, dt, player);
    playerCollideWithPlatforms(reg, dt, player, levels[currentLevel]);
    
}

void gameRender(SDL_Renderer* renderer){
    cameraPosition(reg, game, camera, player, levels[currentLevel]);
    // renderFrameRate(renderer, game);
    renderLevel(renderer, reg, game, camera, levels[currentLevel]);
    renderPlatforms(renderer, reg, game, camera, levels[currentLevel]);
    renderPlayer(renderer, reg, &game, camera, showDevInfo);

    if(showDevInfo){
        renderPlayerCollisionBox(renderer, reg, game, camera, player);
        renderPlatformCollisionBox(renderer, reg, game, camera, levels[currentLevel]);
    }

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