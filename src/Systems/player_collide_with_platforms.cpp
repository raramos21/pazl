#include "player_collide_with_platforms.hpp"

#include "../Components/components.hpp"

// Axis-Aligned Bounding Boxes(AABB) collision detection.
bool AABBCollision(Position aPos, Size aSize, Position bPos, Size bSize){
    float aMaxX = aPos.x + aSize.width;
    float aMaxY = aPos.y;

    float aMinX = aPos.x;
    float aMinY = aPos.y + aSize.height;

    float bMaxX = bPos.x + bSize.width;
    float bMaxY = bPos.y;

    float bMinX = bPos.x;
    float bMinY = bPos.y + bSize.height;
    
    float d1x = bMinX - aMaxX;
    float d1y = bMinY - aMaxY;
    float d2x = aMinX - bMaxX;
    float d2y = aMinY - bMaxY;

    if(d1x > 0.0f || d1y > 0.0f)
        return false;
    
    if(d2x > 0.0f || d2y > 0.0f)
        return false;
    
    return true;
}

bool simpleAABBCollision(Position aPos, Size aSize, Position bPos, Size bSize){
    float aMaxX = aPos.x + aSize.width;
    float aMaxY = aPos.y;

    float aMinX = aPos.x;
    float aMinY = aPos.y + aSize.height;

    float bMaxX = bPos.x + bSize.width;
    float bMaxY = bPos.y;

    float bMinX = bPos.x;
    float bMinY = bPos.y + bSize.height;
    
    if(aMinY <= bMaxY){
        return false;
    }

    if(aMaxY >= bMinY){
        return false;
    }

    if(aMaxX <= bMinX){
        return false;
    }

    if(aMinX >= bMaxX){
        return false;
    }
    
    return true;
}

void playerCollideWithPlatforms(entt::registry &reg, float dt, entt::entity playerEntity, entt::entity levelEntity){
    auto &player         = reg.get<Player>(playerEntity);
    auto &playerPosition = reg.get<Position>(playerEntity);
    auto &playerForce    = reg.get<Force>(playerEntity);

    const auto view      = reg.view<CollisionBox, Size, Position>();

    Position playerCollisionBoxPosition;
    Size     playerCollisionBoxSize;
    Sprite   sprite;

    switch(player.currentAction){
        case action::IDLE:{
            sprite = reg.get<IdleSprite>(playerEntity);
            break;
        }                
        case action::WALK:
            sprite = reg.get<WalkSprite>(playerEntity);
            break;
        case action::RUN:
            sprite = reg.get<RunSprite>(playerEntity);
            break;
        case action::JUMP:
            sprite = reg.get<JumpSprite>(playerEntity);
            break;
    }

    if(player.direction == direction::LOOKING_RIGHT){
        playerCollisionBoxPosition = reg.get<Position>(sprite.rightCollisionBox);
        playerCollisionBoxSize     = reg.get<Size>(sprite.rightCollisionBox);
    } else{
        playerCollisionBoxPosition = reg.get<Position>(sprite.leftCollisionBox);
        playerCollisionBoxSize     = reg.get<Size>(sprite.leftCollisionBox);
    }

    playerCollisionBoxPosition.x += playerPosition.x;
    playerCollisionBoxPosition.y += playerPosition.y;

    // if(player.action != action::JUMP && (player.isFalling || player.isJumping)){

    // } 
    
    player.isColliding       = false;

    for(const entt::entity collisionBoxEntity : view){
        auto &collisionBox = view.get<CollisionBox>(collisionBoxEntity);       

        if(collisionBox.entity == levelEntity){
            const auto collisionBoxPosition = view.get<Position>(collisionBoxEntity);
            const auto collisionBoxSize     = view.get<Size>(collisionBoxEntity);
            
            collisionBox.isColliding = false;

            if(simpleAABBCollision(playerCollisionBoxPosition, playerCollisionBoxSize, collisionBoxPosition, collisionBoxSize)){
                player.isColliding       = true;
                collisionBox.isColliding = true;                
            }
        }        
    }
}