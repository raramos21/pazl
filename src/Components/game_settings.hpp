#ifndef COMP_GAME_SETTINGS_HPP
#define COMP_GAME_SETTINGS_HPP

#include <SDL_ttf.h>

struct GameSettings {
    bool  QUIT;
    int   WIDTH;
    int   HEIGHT;
    int   FRAMES;
    int   FPS_CAP;
    float TICKS_PER_FRAME;
    
    TTF_Font* FONT;

    float AVERAGE_FPS;
    float CURRENT_FPS;
    float CURRENT_PERF;  
};

#endif