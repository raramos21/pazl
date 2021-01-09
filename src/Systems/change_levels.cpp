#include "change_levels.hpp"

#include "player_initial_position.hpp"
#include "camera_initial_position.hpp"

void changeLevels(entt::registry &reg, SDL_Scancode scancode, GameSettings game, Camera &camera, entt::entity player, int & currentLevel, std::vector<entt::entity> levels){
    switch(scancode){
        case SDL_SCANCODE_COMMA:
            currentLevel -= 1;
            if(currentLevel < 0 ){
                currentLevel = 0;
            }

            playerInitialPosition(reg, levels[currentLevel], player);
            cameraInitialPosition(reg, game, camera, player, levels[currentLevel]);
            break;
        case SDL_SCANCODE_PERIOD:
            currentLevel += 1;        
            if(currentLevel >= levels.size()-1){
                currentLevel = levels.size()-1;
            }

            playerInitialPosition(reg, levels[currentLevel], player);
            cameraInitialPosition(reg, game, camera, player, levels[currentLevel]);
            break;
        default:
            break;
    }
}