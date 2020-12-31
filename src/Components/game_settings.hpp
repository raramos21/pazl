#ifndef COMP_GAME_SETTINGS_HPP
#define COMP_GAME_SETTINGS_HPP

#include <SDL_ttf.h>

struct GameSettings {
    int width;
    int height;
    int frames;
    bool quit;
    TTF_Font* gFont;

    float avgFPS;
    float currentFPS;
    float currentPerf;  
};

#endif