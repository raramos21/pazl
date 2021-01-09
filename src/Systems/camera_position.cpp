#include "camera_position.hpp"

void cameraPosition(entt::registry &reg, GameSettings game, Camera &camera, entt::entity playerEntity, entt::entity levelEntity){
    const auto player         = reg.get<Player>(playerEntity);
    const auto playerPosition = reg.get<Position>(playerEntity);

    const auto level          = reg.get<Level>(levelEntity);
    const auto levelPosition  = reg.get<Position>(levelEntity);
    const auto levelSize      = reg.get<Size>(levelEntity);

    if(levelSize.width <= camera.size.width){
        camera.position.x = 0;
    } else if(playerPosition.x > (camera.position.x + camera.size.width)){
        camera.position.x += camera.size.width;
    } else if(playerPosition.x < camera.position.x){
        camera.position.x -= camera.size.width;        
    }
}