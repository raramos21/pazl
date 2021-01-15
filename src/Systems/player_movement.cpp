/*
    Currently all player movement and collisions with the level bounderies happen here.
    But a more robust collision systems where multiple enitities can interact with each other
    will have to be made and this code will have to be refactored.
    - Raul 1/18/2021
*/

#include "player_movement.hpp"

#include "../Components/components.hpp"
#include "../Util/clamp.hpp"

const float X_WALK_FORCE      = 30000.0f;
// const float X_WALK_FORCE      = 3000.0f;

const float X_RUN_FORCE       = 1000.0f;
const float X_RUN_START_FORCE = 35000.0f;
const float X_RUN_MAX_FORCE   = 60000.0f;

const float Y_JUMP_FORCE      = 10.0f;
const float Y_FALL_FORCE      = 30.0f;

const float Y_JUMP_MAX_HEIGHT = 70.0f; 

const float MAX_JUMP_DAMPENING = 0.1f; // MILLISECONDS

float jumpDampening = 0.0f;
// float playerMovementDampening = 0.0f;

void playerMovement(entt::registry & reg, float dt, entt::entity playerEntity){
    auto &player   = reg.get<Player>(playerEntity);
    auto &position = reg.get<Position>(playerEntity);
    auto &velocity = reg.get<Velocity>(playerEntity);
    auto &force    = reg.get<Force>(playerEntity);
    auto &sprite   = reg.get<IdleSprite>(playerEntity);
    
    // maxY -= sprite.size.height;
    // maxX -= sprite.size.width;

    float xAcceleration = 0;
    float yAcceleration = 1;

    // float jumpHeight = maxY - Y_JUMP_MAX_HEIGHT;

    switch(player.currentAction){
        case action::WALK:  
            if(player.direction == direction::LOOKING_RIGHT){
                xAcceleration = 1;
            } else {
                xAcceleration = -1;
            }            
            break;
        case action::RUN:{
            // if(player.direction == direction::LOOKING_RIGHT){
            //     if(player.lastAction == action::RUN){
            //         if(force.x < X_RUN_MAX_FORCE){
            //             force.x += X_RUN_FORCE;
            //         } 
            //     } else {
            //         force.x = X_RUN_START_FORCE;
            //     }                
            // } else {
            //     if(player.lastAction == action::RUN){
            //         if(force.x > -X_RUN_MAX_FORCE){
            //             force.x -= X_RUN_FORCE;
            //         }
            //     } else {
            //         force.x = -X_RUN_START_FORCE;
            //     }                
            // }                   
            break;
        }                
        case action::JUMP: {
            // if(!player.isJumping && jumpDampening >= MAX_JUMP_DAMPENING){
            //     player.isJumping = true;
            //     force.y = -Y_JUMP_FORCE;
            // }     
            if(!player.isJumping){ // && jumpDampening >= MAX_JUMP_DAMPENING){
                player.isJumping = true;
            //     // force.y = -Y_JUMP_FORCE;
                // yAcceleration = 1;
            }  
            break;
        }
        case action::IDLE:
            force.x = 0;
            break;
        case action::ATTACK:
            break;
        default:               
            break;
    }


    // SET X-AXIS VELOCITY & POSITION
    {
        float playerSpeed = 3000;
        xAcceleration *= playerSpeed;
        xAcceleration += -7.5f * velocity.x;

        // Calculate the velocity and force of the players x-axis.
        force.x = xAcceleration * player.mass;
        velocity.x += xAcceleration * dt;
    }

    // SET Y-AXIS VELOCITY & POSITION
    {
        float jumpSpeed = 3000.0f;
        float fallSpeed = 7000.0f;
        // float gravity = 10.0f;        

        if(!player.isJumping && !player.isFalling){
            velocity.y = 0;
        }

        if(player.isJumping && !player.isFalling) {
            // yAcceleration = gravity * velocity.y;
            yAcceleration -= jumpSpeed;
            // printf("acceleration: %f\n", yAcceleration);
        }

        if(player.isFalling){
            yAcceleration += fallSpeed;
            // yAcceleration *= -1;
        }   

        force.y = yAcceleration * player.mass;
        velocity.y += yAcceleration * dt;
    }

    // {
    //     // Player is still in his jumping action but not yet
    //     // falling, so we continue to increase his jump force.
    //     if(player.isJumping && !player.isFalling) {
    //         force.y -= Y_JUMP_FORCE;
    //     }

    //     // Player is now falling so we inverse his y-force until he reaches the ground.
    //     if(player.isFalling){
    //         force.y += Y_FALL_FORCE;
    //     }

    //     // Calculate the velocity and position of the players y-axis.
    //     velocity.y += (force.y/player.mass) * dt;
    //     // position.y += velocity.y * dt;

    //     if(!player.isJumping) jumpDampening += dt;
        
    //     if(position.y < minY){
    //         // Clamp the y-axis position to the levels height
    //         // if level height is shorter than the default jump height.
    //         // So must fall!!
    //         position.y = minY;
    //         force.y = -Y_FALL_FORCE;
    //         player.isFalling = true;
    //     } else if(position.y <= jumpHeight){
    //         // Player has reached the max jump height and is now falling.
    //         player.isFalling = true;
    //     } else if(position.y > maxY){
    //         // Player has reached the floor/maxY so he has stopped falling and jumping.
    //         jumpDampening = 0.0f;
    //         player.isFalling = false;
    //         player.isJumping = false;
    //         force.y = 0.0f;
    //         position.y = maxY;
    //     }       
    // }
    

}