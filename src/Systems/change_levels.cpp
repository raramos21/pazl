#include "change_levels.hpp"

void changeLevels(SDL_Scancode scancode, int & currentLevel, int totalLevels){
    switch(scancode){
        case SDL_SCANCODE_COMMA:
            currentLevel -= 1;
            if(currentLevel < 0 ){
                currentLevel = 0;
            }
            break;
        case SDL_SCANCODE_PERIOD:
            currentLevel += 1;        
            if(currentLevel >= totalLevels){
                currentLevel = totalLevels;
            }
            break;
        default:
            break;
    }  
}