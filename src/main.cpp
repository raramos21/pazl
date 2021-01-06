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
#include "Core/render.hpp"
#include "Core/input_handler.hpp"
#include "Components/game_settings.hpp"

int main(int argc, char* args[]) {
    InputHandler inputHandler;
    entt::registry reg;
    GameSettings game;
    game.WIDTH           = 1500;
    game.HEIGHT          =  800;
    game.FRAMES          = 0;
    game.FPS_CAP         = 120;
    game.TICKS_PER_FRAME = 1000 / game.FPS_CAP;
    game.QUIT            = false;
    game.FONT            = NULL;
    game.CURRENT_FPS     = 0.0;
    game.AVERAGE_FPS     = 0.0;
    game.CURRENT_PERF    = 0.0;

    
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;

    SDL_CHECK(SDL_Init(SDL_INIT_VIDEO));        
    SDL_CHECK((IMG_Init(imgFlags) & imgFlags));
    SDL_CHECK(TTF_Init());
    SDL_CHECK(loadFramerateFont(&game));
    SDL_Window* gWindow{SDL_CHECK(SDL_CreateWindow("Pazl", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, game.WIDTH, game.HEIGHT, SDL_WINDOW_SHOWN))};
    SDL_Renderer* gRenderer{SDL_CHECK(SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED ))}; //| SDL_RENDERER_PRESENTVSYNC))};     
    
    gameInit(gRenderer, reg, &game);

    double t = 0.0;
    float dt = 0.1;
    
    double currentTime = SDL_GetTicks() / 1000.0;
    double accumulator = 0.0;

    Uint32 totalFrameTicks = 0; // @Temp: Testing framefrate without a fixed time step
    Uint32 totalFrames     = 0; // will be changed when adding a more robust game loop.

    while(!game.QUIT) {
        Uint32 startTicks = SDL_GetTicks(); // @Temp
        Uint64 startPerf  = SDL_GetPerformanceCounter(); // @Temp

        totalFrames++;

        double newTime = startTicks / 1000.0;
        double frameTime = newTime - currentTime;

        // if(frameTime > 0.25) frameTime = 0.25;

        currentTime = newTime;
        accumulator += frameTime;

        SDL_Event e;
        while(SDL_PollEvent(&e) != 0){
            switch(e.type){
                case SDL_QUIT:
                    game.QUIT = true;
                    break;
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    inputHandler.handleInputEvent(e.key.keysym.scancode);
                    
                    break;
            }
        }

        gameInput(reg, e.key.keysym.scancode, inputHandler); 

        while(accumulator >= dt) {
            // previousState = currentState;
            gameLogic(reg, t, dt);
            accumulator -= dt;
            t += dt;            
        }

        SDL_SetRenderDrawColor(gRenderer, 0xf3, 0xf4, 0xf6, 255);
        SDL_RenderClear(gRenderer);

        gameRender(gRenderer, reg, &game);                


        Uint32 endTicks  = SDL_GetTicks();                                      // @Temp
        Uint64 endPerf   = SDL_GetPerformanceCounter();                         // @Temp
        Uint64 framePerf = endPerf - startPerf;                                 // @Temp
        float frameTimeF = (endTicks - startTicks) / 1000.0f;                   // @Temp
        totalFrameTicks += endTicks - startTicks;                               // @Temp
        game.CURRENT_FPS  = 1.0f / frameTimeF;                                  // @Todo: Currently cannot accurately calculate fps
        game.AVERAGE_FPS  = 1000.0f / ((float)totalFrameTicks / totalFrames);
        game.CURRENT_PERF = framePerf;

        // renderFrameRate(gRenderer, &game);

        SDL_RenderPresent(gRenderer);
        
        
        if( (frameTimeF * 1000) < game.TICKS_PER_FRAME ) 
        {            
            //Sleep the remaining frame time
            SDL_Delay( game.TICKS_PER_FRAME - (frameTimeF * 1000) );
        } 
        
    }

    gameQuit(gWindow, gRenderer, reg, &game);

    return 0;
}