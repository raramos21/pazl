#include "change_levels.hpp"

#include "player_initial_position.hpp"

void changeLevels(entt::registry &reg, SDL_Scancode scancode, entt::entity player, int & currentLevel, std::vector<entt::entity> levels){
    switch(scancode){
        case SDL_SCANCODE_COMMA:
            currentLevel -= 1;
            if(currentLevel < 0 ){
                currentLevel = 0;
            }

            playerInitialPosition(reg, levels[currentLevel], player);
            break;
        case SDL_SCANCODE_PERIOD:
            currentLevel += 1;        
            if(currentLevel >= levels.size()-1){
                currentLevel = levels.size()-1;
            }

            playerInitialPosition(reg, levels[currentLevel], player);
            break;
        default:
            break;
    }
}