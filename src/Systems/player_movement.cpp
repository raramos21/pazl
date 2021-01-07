#include "player_movement.hpp"

#include "../Components/components.hpp"

const float X_WALK_FORCE      = 3500.0f;

const float X_RUN_FORCE       = 100.0f;
const float X_RUN_START_FORCE = 3500.0F;
const float X_RUN_MAX_FORCE   = 6000.0F;

const float Y_JUMP_FORCE      = 2800.0f;
const float Y_FALL_FORCE      = 3300.0f;

const float Y_FLOOR           = 350.0f; 
const float Y_CEILING         = 300.0f; 

void playerMovement(entt::registry & reg, float dt){
    const auto view = reg.view<Player, Position, Velocity, Force>();
    for(const entt::entity e: view) {
        auto &player = view.get<Player>(e);
        auto &position = view.get<Position>(e);
        auto &velocity = view.get<Velocity>(e);
        auto &force = view.get<Force>(e);

        switch(player.currentAction){
            case WALK_RIGHT:
                force.x = X_WALK_FORCE;
                break;
            case WALK_LEFT:
                force.x = -X_WALK_FORCE;
                break;
            case RUN_RIGHT:{
                if(player.lastAction == RUN_RIGHT){
                    if(force.x < X_RUN_MAX_FORCE){
                        force.x += X_RUN_FORCE;
                    } 
                } else {
                    force.x = X_RUN_START_FORCE;
                }                
                break;
            }                
            case RUN_LEFT:{
                if(player.lastAction == RUN_LEFT){
                    if(force.x > -X_RUN_MAX_FORCE){
                        force.x -= X_RUN_FORCE;
                    }
                } else {
                    force.x = -X_RUN_START_FORCE;
                }                
                break;
            }                
            case JUMP: {
                if(!player.isJumping){
                    player.isJumping = true;
                    force.y = -Y_JUMP_FORCE;
                }          
                break;
            }
            case IDLE:
                force.x = 0;
                force.y = 0;
                break;
            case ATTACK:
                break;
            case RESET:
                position.y = Y_FLOOR;
                force.x = 0;
                force.y = 0;
                break;
            default:               
                break;
        }
        
        velocity.x = (force.x/player.mass) * dt;
        position.x += velocity.x * dt;

        if(player.isJumping && !player.isFalling) {
            force.y -= Y_JUMP_FORCE;
        }

        if(player.isFalling){
            force.y += Y_FALL_FORCE;
        }

        velocity.y = (force.y/player.mass) * dt;
        position.y += velocity.y * dt;

        if(position.y < Y_CEILING){
            player.isFalling = true;
        }

        if(position.y > Y_FLOOR){
            player.isFalling = false;
            player.isJumping = false;
            force.y = 0.0f;
            position.y = Y_FLOOR;
        }       
    }
}