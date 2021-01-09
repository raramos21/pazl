#include "camera_initial_position.hpp"

void cameraInitialPosition(entt::registry &reg, GameSettings game, Camera &camera, entt::entity playerEntity, entt::entity levelEntity){
    const auto level          = reg.get<Level>(levelEntity);
    const auto levelPosition  = reg.get<Position>(levelEntity);
    const auto levelSize      = reg.get<Size>(levelEntity);

    if(levelSize.width <= camera.size.width){
        camera.position.x = 0;
    } else{
        camera.position.x = levelPosition.x;
    }
}