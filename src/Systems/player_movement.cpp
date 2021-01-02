#include "player_movement.hpp"

#include "../Components/player.hpp"

void playerMovement(entt::registry & reg, float dt){
    const auto view = reg.view<Player, Position, Velocity, Force>();
    for(const entt::entity e: view) {
        auto &player = view.get<Player>(e);
        auto &position = view.get<Position>(e);
        auto &velocity = view.get<Velocity>(e);
        auto &force = view.get<Force>(e);

        switch(player.currentAction){
            case MOVE_RIGHT:
                if(player.lastAction == MOVE_RIGHT){
                    if(force.x < 2000.0f){
                        force.x += 30.0f;
                    }                    
                } else {
                    force.x = 400.0f;
                }                
                velocity.x = (force.x/player.mass) * dt;
                position.x += velocity.x * dt;
                break;
            case MOVE_LEFT:
                if(player.lastAction == MOVE_LEFT){
                    if(force.x > -2000.0f){
                        force.x -= 30.0f;
                    }
                } else {
                    force.x = -400.0f;
                }                
                velocity.x = (force.x/player.mass) * dt;
                position.x += velocity.x * dt;
                break;
            case JUMP:
                force.y -= 400.0f;
                velocity.y = (force.y/player.mass) * dt;
                position.y += velocity.y * dt;
                break;
            case RUN:
                
                break;
            case IDLE:
                
                break;
            case ATTACK:
                
                break;
            default:               
                break;
        }
    }
}