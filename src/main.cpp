#include <iostream>
#include <stdio.h>
#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <entt/entt.hpp>

#include "Util/sdl_check.hpp"
#include "Core/game.hpp"
#include "Components/components.hpp"

// enum JoyStickDirection{
//     LEFT,
//     RIGHT
// }

// struct Controller{
//     JoyStickDirectionxAxis
// }

int main(int argc, char* args[]) {
    const int JOYSTICK_DEAD_ZONE = 10000;
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;

    SDL_CHECK(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK));        
    SDL_CHECK((IMG_Init(imgFlags) & imgFlags));
    SDL_CHECK(TTF_Init());
    SDL_CHECK(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"));

    GameSettings game = gameInit();
    

    SDL_Window* gWindow{SDL_CHECK(SDL_CreateWindow("Pazl", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, game.WIDTH, game.HEIGHT, SDL_WINDOW_SHOWN))};
    SDL_Renderer* gRenderer{SDL_CHECK(SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))};     
    SDL_Joystick* gGameController = NULL;

    if(SDL_NumJoysticks() < 1){
        printf("WARNING: No joysticks connected!\n");
    } else{
        gGameController = SDL_JoystickOpen(0);
        if(gGameController == NULL){
            printf( "Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError() );
        }
    }
    
    gameCreateEntities(gRenderer);

    double t = 0.0;
    float dt = 1.0f/120.0f;
    
    double currentTime = SDL_GetTicks() / 1000.0;
    double accumulator = 0.0;

    Uint32 totalFrameTicks = 0; // @Temp: Testing framefrate without a fixed time step
    Uint32 totalFrames     = 0; // will be changed when adding a more robust game loop.

    while(!game.QUIT) {
        SDL_SetRenderDrawColor(gRenderer, 0xf3, 0xf4, 0xf6, 255);       
        SDL_RenderClear(gRenderer); 

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
                    gameImmediateInput(e.key.keysym.scancode);
                    break;
                case SDL_KEYUP:
                    gameDefaultInput(e.key.keysym.scancode);
                    break;
                case SDL_JOYAXISMOTION:
                    if( e.jaxis.which == 0 ){                        
                        //X axis motion
                        if( e.jaxis.axis == 0 ){
                            //Left of dead zone
                            if( e.jaxis.value < -JOYSTICK_DEAD_ZONE ){
                                printf("x direction: %i\n", -1);
                            }
                            //Right of dead zone
                            else if( e.jaxis.value > JOYSTICK_DEAD_ZONE ){
                                printf("x direction: %i\n", 1);
                            } else{
                                printf("x direction: %i\n", 0);
                            }
                        } else if( e.jaxis.axis == 1 ){
                            //Below of dead zone
                            if( e.jaxis.value < -JOYSTICK_DEAD_ZONE ){
                                printf("y direction: %i\n", -1);
                            }
                            //Above of dead zone
                            else if( e.jaxis.value > JOYSTICK_DEAD_ZONE ){
                                printf("y direction: %i\n", 1);
                            }
                            else{
                                printf("y direction: %i\n", 0);
                            }
                        }
                    }
                    break;
            }
        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        gameInput(e.key.keysym.scancode, currentKeyStates); 
        // gameInputController();

        while(accumulator >= dt) {
            gameLogic(t, dt);
            accumulator -= dt;
            t += dt;            
        }
    
        gameRender(gRenderer);                

        Uint32 endTicks  = SDL_GetTicks();                                      // @Temp
        Uint64 endPerf   = SDL_GetPerformanceCounter();                         // @Temp
        Uint64 framePerf = endPerf - startPerf;                                 // @Temp
        float frameTimeF = (endTicks - startTicks) / 1000.0f;                   // @Temp
        totalFrameTicks += endTicks - startTicks;                               // @Temp
        game.CURRENT_FPS  = 1.0f / frameTimeF;                                  // @Todo: Currently cannot accurately calculate fps
        game.AVERAGE_FPS  = 1000.0f / ((float)totalFrameTicks / totalFrames);
        game.CURRENT_PERF = framePerf;

        SDL_RenderPresent(gRenderer);
        
        if( (frameTimeF * 1000) < game.TICKS_PER_FRAME ) 
        {            
            //Sleep the remaining frame time
            SDL_Delay( game.TICKS_PER_FRAME - (frameTimeF * 1000) );
        }           
    }

    SDL_JoystickClose( gGameController );
    gGameController = NULL;
    gameQuit(gWindow, gRenderer);

    return 0;
}