/*
    Currently all player movement and collisions with the level bounderies happen here.
    But a more robust collision systems where multiple enitities can interact with each other
    will have to be made and this code will have to be refactored.
    - Raul 1/18/2021
*/

#include "player_movement.hpp"

#include "../Components/components.hpp"

// const float X_WALK_FORCE      = 16000.0f;
const float X_WALK_FORCE      = 30000.0f;

const float X_RUN_FORCE       = 1000.0f;
const float X_RUN_START_FORCE = 35000.0f;
const float X_RUN_MAX_FORCE   = 60000.0f;

const float Y_JUMP_FORCE      = 5000.0f;
const float Y_FALL_FORCE      = 6000.0f;

const float Y_JUMP_MAX_HEIGHT = 100.0f; 

const float MAX_JUMP_DAMPENING = 0.1f; // MILLISECONDS

float jumpDampening = 0.0f;

void playerMovement(entt::registry & reg, float dt, entt::entity levelEntity){
    const auto level         = reg.get<Level>(levelEntity);
    const auto levelSize     = reg.get<Size>(levelEntity);
    const auto levelPosition = reg.get<Position>(levelEntity);
    
    const auto view  = reg.view<Player, Position, Velocity, Force, IdleSprite>();

    float maxX    = levelSize.width + levelPosition.x;
    float minX    = levelPosition.x;
    float maxY    = level.floor;                          
    float minY    = levelPosition.y;

    for(const entt::entity e: view) {
        auto &player   = view.get<Player>(e);
        auto &position = view.get<Position>(e);
        auto &velocity = view.get<Velocity>(e);
        auto &force    = view.get<Force>(e);
        auto &sprite   = view.get<IdleSprite>(e);
        
        maxY -= sprite.size.height;
        maxX -= sprite.size.width;

        float jumpHeight = maxY - Y_JUMP_MAX_HEIGHT;

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
                if(!player.isJumping && jumpDampening >= MAX_JUMP_DAMPENING){
                    player.isJumping = true;
                    force.y = -Y_JUMP_FORCE;
                }     
                break;
            }
            case IDLE:
                force.x = 0;
                // force.y = 0;
                break;
            case ATTACK:
                break;
            case RESET:
                position.y = maxY;
                force.x = 0;
                force.y = 0;
                break;
            default:               
                break;
        }


        // SET X-AXIS VELOCITY & POSITION
        {    
            // Calculate the velocity and position of the players x-axis.
            velocity.x = (force.x/player.mass) * dt;
            position.x += velocity.x * dt;

            // Clamp player to right edge of level if position
            // is greater than the levels width.
            if(position.x > maxX){
                position.x = maxX;
            }

            // Same as before but on the left side.
            if(position.x < minX){
                position.x = minX;
            }
        }       


        // SET Y-AXIS VELOCITY & POSITION
        {
            // Player is still in his jumping action but not yet
            // falling, so we continue to increase his jump force.
            if(player.isJumping && !player.isFalling) {
                force.y -= Y_JUMP_FORCE;
            }

            // Player is now falling so we inverse his y-force until he reaches the ground.
            if(player.isFalling){
                force.y += Y_FALL_FORCE;
            }

            // Calculate the velocity and position of the players y-axis.
            velocity.y = (force.y/player.mass) * dt;
            position.y += velocity.y * dt;

            if(!player.isJumping) jumpDampening += dt;
            
            if(position.y < minY){
                // Clamp the y-axis position to the levels height
                // if level height is shorter than the default jump height.
                // So must fall!!
                position.y = minY;
                force.y = -Y_FALL_FORCE;
                player.isFalling = true;
            } else if(position.y <= jumpHeight){
                // Player has reached the max jump height and is now falling.
                player.isFalling = true;
            } else if(position.y > maxY){
                // Player has reached the floor/maxY so he has stopped falling and jumping.
                jumpDampening = 0.0f;
                player.isFalling = false;
                player.isJumping = false;
                force.y = 0.0f;
                position.y = maxY;
            }       
        }
      
    }
}