#include "set_camera.hpp"

void setCamera(SDL_Renderer * renderer, entt::registry &reg, GameSettings game, Camera &camera, entt::entity playerEntity, entt::entity levelEntity){
    const auto player         = reg.get<Player>(playerEntity);
    const auto playerPosition = reg.get<Position>(playerEntity);

    const auto level          = reg.get<Level>(levelEntity);
    const auto levelPosition  = reg.get<Position>(levelEntity);
    const auto levelSize      = reg.get<Size>(levelEntity);

    // if(playerPosition.x < camera.position.x && player.currentAction == IDLE){
    //     camera.position.x = playerPosition.x;
    // } else 

    if(levelSize.width <= camera.size.width){
        camera.position.x = 0;
    }else if(playerPosition.x > (camera.position.x + camera.size.width)){
        camera.position.x = playerPosition.x;
    } else if(playerPosition.x < camera.position.x){
        float newXPosition = playerPosition.x - camera.size.width; 
        if(newXPosition - camera.position.x < camera.size.width){
            camera.position.x = levelPosition.x;
        } else {
            camera.position.x = newXPosition;
        }
        
    }

    

    
}