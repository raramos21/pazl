#include "player_collide_with_platforms.hpp"

#include "../Components/components.hpp"
#include "../Systems/player_movement.hpp"
#include "../Util/clamp.hpp"

#include <SDL.h>

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

float sweptAABB(Position aPos, Size aSize, Velocity aVel, Position bPos, Size bSize, float &normalX, float &normalY){
    // xInvEntry and yInvEntry both specify how far away the closest edges of the objects are from each other.
    // xInvExit and yInvExit is the distance to the far side of the object.
    float xInvEntry, yInvEntry;
    float xInvExit, yInvExit;

    if(aVel.x > 0.0f){
        xInvEntry = bPos.x - (aPos.x + aSize.width);
        xInvExit  = (bPos.x + bSize.width) - aPos.x;
    } else{
        xInvEntry = (bPos.x + bSize.width) - aPos.x;
        xInvExit  = bPos.x - (aPos.x + aSize.width);
    }

    if(aVel.y > 0.0f){
        yInvEntry = bPos.y - (aPos.y + aSize.height);
        yInvExit  = (bPos.y + bSize.height) - aPos.y;
    } else{
        yInvEntry = (bPos.y + bSize.height) - aPos.y;
        yInvExit  = bPos.y - (aPos.y + aSize.height);
    }

    float xEntry, yEntry;
    float xExit, yExit;

    if(aVel.x == 0.0f){
        xEntry = -std::numeric_limits<float>::infinity();
        xExit  =  std::numeric_limits<float>::infinity();
    } else{
        xEntry = xInvEntry / aVel.x;
        xExit  = xInvExit  / aVel.x;
    }

    if(aVel.y == 0.0f){
        yEntry = -std::numeric_limits<float>::infinity();
        yExit  =  std::numeric_limits<float>::infinity();
    } else{
        yEntry = yInvEntry / aVel.y;
        yExit  = yInvExit  / aVel.y;
    }

    // if(xEntry > 1.0f) xEntry = -std::numeric_limits<float>::infinity();
    // if(yEntry > 1.0f) yEntry = -std::numeric_limits<float>::infinity();

    float entryTime = std::max(xEntry, yEntry);
    float exitTime  = std::min(xExit, yExit);

    float aMaxX = aPos.x + aSize.width;
    float aMaxY = aPos.y;

    float aMinX = aPos.x;
    float aMinY = aPos.y + aSize.height;

    float bMaxX = bPos.x + bSize.width;
    float bMaxY = bPos.y;

    float bMinX = bPos.x;
    float bMinY = bPos.y + bSize.height;

    normalX = 0.0f;
    normalY = 0.0f;

    // if(entryTime > exitTime || (xEntry < 0.0f && yEntry < 0.0f)){ // || xEntry > 1.0f || yEntry > 1.0f){
    if(entryTime > exitTime || (xEntry < 0.0f && yEntry < 0.0f) || xEntry > 1.0f || yEntry > 1.0f){
        return 1.0f;
    }
    // if(xEntry < 0.0f){
    //     if(aMaxX < bMinX || aMinX > bMaxX) return 1.0f;
    // }
    // if(xEntry < 0.0f){
    //     if(aMaxY < bMinY || aMinY > bMaxY) return 1.0f;
    // }
    else{
        if(xEntry > yEntry){
            if(xInvEntry < 0.0f){
                normalX = 1.0f;
                normalY = 0.0f;
            } else{
                normalX = -1.0f;
                normalY = 0.0f;
            }
        } else{
            if(yInvEntry < 0.0f){
                normalX = 0.0f;
                normalY = 1.0f;
            } else{
                normalX = 0.0f;
                normalY = -1.0f;
            }
        }
        return entryTime;
    }
}

void calculateAABBDistance(Position aPos, Size aSize, Position bPos, Size bSize, float &dx, float &dy){
    dx = 0;
    dy = 0;

    if(aPos.x < bPos.x){
        dx = bPos.x - (aPos.x + aSize.width);
    } else if(aPos.x > bPos.x){
        dx = aPos.x - (bPos.x + bSize.width);
    }

    if(aPos.y < bPos.y){
        dy = bPos.y - (aPos.y + aSize.height);
    } else if(aPos.x > bPos.x){
        dx = aPos.y - (bPos.y + bSize.height);
    }
}

bool equal(Position a, Position b){
    if(a.x != b.x) return false;
    if(a.y != b.y) return false;
    return true;
}

/*
vector inner(vector a, vector b){
    return a.x*b.x + a.y*b.y;
}

vector r = {0, 1}

velocity = velocity - 2*inner(velocity, r)*r

coefficient of restitution
*/

float lerp(float a, float b, float f){
    return (a * (1.0f - f)) + (b * f);
}

Position lerp(Position a, Position b, float f){
    Position result;

    result.x = lerp(a.x, b.x, f);
    result.y = lerp(a.y, b.y, f);
    return result;
}

inline
Position operator-(Position const &a, Position const &b){
    return Position{a.x -b.x, a.y - b.y};
}

void playerCollideWithPlatforms(entt::registry &reg, float dt, entt::entity playerEntity, entt::entity levelEntity){
    auto &player         = reg.get<Player>(playerEntity);
    auto &playerPosition = reg.get<Position>(playerEntity);
    auto &playerForce    = reg.get<Force>(playerEntity);
    auto &playerVelocity = reg.get<Velocity>(playerEntity);

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
        default:
            break;
    }

    if(player.currentAction != action::JUMP && (player.isFalling || player.isJumping)){
        sprite = reg.get<JumpSprite>(playerEntity);
    }

    sprite = reg.get<WalkSprite>(playerEntity);

    if(player.direction == direction::LOOKING_RIGHT){
        playerCollisionBoxPosition = reg.get<Position>(sprite.rightCollisionBox);
        playerCollisionBoxSize     = reg.get<Size>(sprite.rightCollisionBox);
    } else{
        playerCollisionBoxPosition = reg.get<Position>(sprite.leftCollisionBox);
        playerCollisionBoxSize     = reg.get<Size>(sprite.leftCollisionBox);
    }


    playerCollisionBoxPosition.x += playerPosition.x;
    playerCollisionBoxPosition.y += playerPosition.y;
    player.isColliding = false;
    // player.isGoingToCollide       = false;

    float collisionTime = 0;
    float normalX, normalY;

    for(const entt::entity collisionBoxEntity : view){
        auto &collisionBox = view.get<CollisionBox>(collisionBoxEntity);

        if(collisionBox.entity == levelEntity){
            auto &collisionBoxPosition = view.get<Position>(collisionBoxEntity);
            const auto collisionBoxSize     = view.get<Size>(collisionBoxEntity);

            collisionBox.isColliding = false;
            collisionBox.isGoingToCollide = false;

            if(simpleAABBCollision(playerCollisionBoxPosition, playerCollisionBoxSize, collisionBoxPosition, collisionBoxSize)){
                player.isColliding       = true;
                collisionBox.isColliding = true;
            }

            collisionTime = sweptAABB(playerCollisionBoxPosition, playerCollisionBoxSize, playerVelocity, collisionBoxPosition, collisionBoxSize, normalX, normalY);
        }
    }


    {
        const auto level         = reg.get<Level>(levelEntity);
        const auto levelSize     = reg.get<Size>(levelEntity);
        const auto levelPosition = reg.get<Position>(levelEntity);

        float xMax    = levelPosition.x + levelSize.width;
        float xMin    = levelPosition.x;
        float yMin    = levelPosition.y + levelSize.height;
        float yMax    = levelPosition.y;

        yMin -= sprite.size.height;
        xMax -= sprite.size.width;

        if(collisionTime < 0.01f){
            playerPosition.x += playerVelocity.x * collisionTime;
            playerPosition.y += playerVelocity.y * collisionTime;
            float remainingTime = 1.0f - collisionTime;

            float dotprod = (playerVelocity.x * normalY + playerVelocity.y * normalX) * remainingTime;
            playerVelocity.x = dotprod * normalY;
            playerVelocity.y = dotprod * normalX;
        } else {
            playerPosition.x += 0.5*(playerForce.x/player.mass)*(dt*dt) + playerVelocity.x*dt;
            playerPosition.y += 0.5*(playerForce.y/player.mass)*(dt*dt) + playerVelocity.y*dt;
        }

        clamp(&playerPosition, xMin, xMax, yMin, yMax);

        float jumpHeight = yMin - 150.0f;
        if(playerPosition.y < jumpHeight){
            playerPosition.y = jumpHeight;
            player.isFalling = true;
            playerForce.y = 0;
        } else if(playerPosition.y == yMax){
            player.isFalling = true;
        } else if(playerPosition.y == yMin){
            player.isJumping = false;
            player.isFalling = false;
            playerForce.y = 0;
        }
    }
}