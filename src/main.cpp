#include <iostream>
#include <stdio.h>
#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <entt/entt.hpp>

#include "Math/Mat44.hpp"
#include "Util/sdl_check.hpp"
#include "Core/game.hpp"
#include "Core/load.hpp"
#include "Components/game_settings.hpp"

int main(int argc, char* args[]) {
    GameSettings game;
    game.width = 1500;
    game.height =  800;
    game.frames = 0;
    game.quit = false;
    game.gFont = NULL;

    entt::registry reg;
    
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;

    SDL_CHECK(SDL_Init(SDL_INIT_VIDEO));        
    SDL_CHECK((IMG_Init(imgFlags) & imgFlags));
    SDL_CHECK(TTF_Init());
    SDL_CHECK(loadFramerateFont(&game));
    SDL_Window* gWindow{SDL_CHECK(SDL_CreateWindow("Pazl", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, game.width, game.height, SDL_WINDOW_SHOWN))};
    SDL_Renderer* gRenderer{SDL_CHECK(SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))};     
    
    gameInit(gRenderer, reg, &game);

    Uint32 totalFrameTicks = 0; // @Temp: Testing framefrate without a fixed time step
    Uint32 totalFrames     = 0; // will be changed when adding a more robust game loop.

    while(!game.quit) {
        Uint32 startTicks = SDL_GetTicks(); // @Temp
        Uint64 startPerf  = SDL_GetPerformanceCounter(); // @Temp
        totalFrames++;

        SDL_Event e;
        while(SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT){
                game.quit = true;
            } else if(e.type == SDL_KEYDOWN){
                gameInput(reg, e.key.keysym.scancode);               
            }
        }

        Uint32 endTicks  = SDL_GetTicks(); // @Temp
        Uint64 endPerf   = SDL_GetPerformanceCounter(); // @Temp
        Uint64 framePerf = endPerf - startPerf; // @Temp
        float frameTime  = (endTicks - startTicks) / 1000.0f; // @Temp
        totalFrameTicks += endTicks - startTicks; // @Temp

        game.currentFPS  = 1.0f / frameTime;
        game.avgFPS      = 1000.0f /((float) totalFrameTicks / totalFrames);
        game.currentPerf = framePerf;

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 255);
        SDL_RenderClear(gRenderer);
        
        gameRender(gRenderer, reg, &game);
        
        SDL_RenderPresent(gRenderer);
    }

    gameQuit(gWindow, gRenderer, reg, &game);

    return 0;
}