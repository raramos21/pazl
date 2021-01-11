#include "render.hpp"

#include "../Components/components.hpp"

void renderText(SDL_Renderer *renderer, TTF_Font *gFont, SDL_Rect dest,  SDL_Color color, const std::string text){
	SDL_Color shadowColor = { 255, 255, 255 };
    SDL_Surface* surf = TTF_RenderText_Solid(gFont, text.c_str(), color);
    SDL_Surface* shadowSurf = TTF_RenderText_Solid(gFont, text.c_str(), shadowColor);

	dest.w = surf->w+1;
	dest.h = surf->h+1;

    SDL_Texture* shadowTex = SDL_CreateTextureFromSurface(renderer, shadowSurf);
	// SDL_RenderCopy(renderer, shadowTex, NULL, &dest);

    dest.w -= 1;
    dest.h -= 1;

	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_RenderCopy(renderer, tex, NULL, &dest);

	SDL_DestroyTexture(tex);
    SDL_FreeSurface(surf);

    SDL_DestroyTexture(shadowTex);	
    SDL_FreeSurface(shadowSurf);
}

void renderFrameRate(SDL_Renderer *renderer, GameSettings *game) {
    SDL_Rect dest = { 10, 10, 0, 0 };
    SDL_Color color = { 0, 0xcd, 0x10 };
    renderText(renderer, game->FONT, dest, color, "Current FPS: " + std::to_string(game->CURRENT_FPS));
    dest.y += 24;
    renderText(renderer, game->FONT, dest, color, "Average FPS: " + std::to_string(game->AVERAGE_FPS));
    dest.y += 24;
    renderText(renderer, game->FONT, dest, color, "Current Perf: " + std::to_string(game->CURRENT_PERF));
}

std::string getPlayerActionString(action::Action action){
    std::string actionString = "NONE";
    switch(action){
        case action::WALK:
            actionString = "WALK";
            break;
        case action::JUMP:
            actionString = "JUMP";
            break;
        case action::RUN:
            actionString = "RUN";
            break;
        case action::IDLE:
            actionString = "IDLE";
            break;
        case action::ATTACK:
            actionString = "ATTACK";
            break;
        default:
            actionString = "NONE";
            break;
    }
    return actionString;
}

std::string getPlayerDirectionString(direction::Direction direction){
    std::string directionString = "NONE";
    switch(direction){
        case direction::LOOKING_RIGHT:
            directionString = "LOOKING RIGHT";
            break;
        case direction::LOOKING_LEFT:
            directionString = "LOOKING LEFT";
            break;
        default:
            directionString = "NONE";
            break;
    }
    return directionString;
}

void renderSprite(SDL_Renderer *renderer, GameSettings *game, Player player, Position position, Sprite sprite, Camera camera, int & total_sprite_frames, bool showDevInfo){
    total_sprite_frames = sprite.total_frames;
    int clip = game->FRAMES;
    // int clip = 0;

    SDL_Rect currentClip = sprite.spriteClips[clip];
    SDL_FRect renderQuad = {(position.x - camera.position.x), (position.y - camera.position.y), (float) sprite.size.width, (float) sprite.size.height};

    if(showDevInfo){
        // Render green rectangle around sprite.
        SDL_FRect fillRectF = {(position.x - camera.position.x), (position.y - camera.position.y), (float) sprite.size.width, (float) sprite.size.height};
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
        SDL_RenderDrawRectF(renderer, &fillRectF);

        // Render current clip number.
        SDL_Color color       = { 0xff, 0xff, 0xff };
        SDL_Rect textPosition = {(int) (position.x - camera.position.x + 5), (int) (position.y - camera.position.y + 2), 0, 0};
        renderText(renderer, game->FONT, textPosition, color, std::to_string(clip));    
    }    
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
    if(player.direction == direction::LOOKING_LEFT){
        SDL_RenderCopyExF(renderer, sprite.textureSheet, &currentClip, &renderQuad, 0, NULL, SDL_FLIP_HORIZONTAL);                 
    } else{
        SDL_RenderCopyExF(renderer, sprite.textureSheet, &currentClip, &renderQuad, 0, NULL, SDL_FLIP_NONE);                
    }             
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void renderPlayer(SDL_Renderer *renderer, entt::registry &reg, GameSettings *game, Camera camera, bool showDevInfo){
    const auto view = reg.view<Player, Position, Velocity, Force, IdleSprite, RunSprite, WalkSprite, JumpSprite>();
    for(const entt::entity e : view){
        const auto player     = view.get<Player>(e);
        const auto position   = view.get<Position>(e);
        const auto velocity   = view.get<Velocity>(e);
        const auto force      = view.get<Force>(e);
        const auto walkSprite = view.get<WalkSprite>(e);
        const auto runSprite  = view.get<RunSprite>(e);
        const auto idleSprite = view.get<IdleSprite>(e);
        const auto jumpSprite = view.get<JumpSprite>(e);

        int total_sprite_frames = 1;
        
        if(player.isJumping || player.isFalling){
            renderSprite(renderer, game, player, position, jumpSprite, camera, total_sprite_frames, showDevInfo);            
        } else {
            if(player.currentAction == action::WALK){      
                renderSprite(renderer, game, player, position, walkSprite, camera, total_sprite_frames, showDevInfo);
            }
            if(player.currentAction == action::RUN){
                renderSprite(renderer, game, player, position, runSprite, camera, total_sprite_frames, showDevInfo);
            }
            if(player.currentAction == action::IDLE || player.currentAction == action::JUMP){
                renderSprite(renderer, game, player, position, idleSprite, camera, total_sprite_frames, showDevInfo);
            }
        }

        game->FRAMES += 1;

        if(game->FRAMES >= total_sprite_frames){
            game->FRAMES = 0;
        } 
    }   
}

void renderPlayerInfo(SDL_Renderer *renderer, entt::registry &reg, GameSettings game){
    const auto view = reg.view<Player, Position, Velocity, Force>();
    for(const entt::entity e : view){
        const auto player   = view.get<Player>(e);
        const auto position = view.get<Position>(e);
        const auto velocity = view.get<Velocity>(e);
        const auto force    = view.get<Force>(e);
        
        std::string lastAction    = getPlayerActionString(player.lastAction);
        std::string currentAction = getPlayerActionString(player.currentAction);
        std::string direction     = getPlayerDirectionString(player.direction);

        SDL_Rect fillRect = {0, 0, 550, 220};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(renderer, &fillRect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

        SDL_Color color       = { 0x29, 0x2f, 0x36 };
        SDL_Rect textPosition = {10, 10, 0, 0};

        renderText(renderer, game.FONT, textPosition, color, "Direction: " + direction);
        textPosition.y += 25;
        renderText(renderer, game.FONT, textPosition, color, "Current action: " + currentAction);
        textPosition.y += 25;
        renderText(renderer, game.FONT, textPosition, color, "Last action: " + lastAction);
        textPosition.y += 25;
        renderText(renderer, game.FONT, textPosition, color, "Position x: " + std::to_string(position.x) + ", y: " + std::to_string(position.y));
        textPosition.y += 25;
        renderText(renderer, game.FONT, textPosition, color, "Health: " + std::to_string(player.health));
        textPosition.y += 25;
        renderText(renderer, game.FONT, textPosition, color, "Mass: " + std::to_string(player.mass));
        textPosition.y += 25;
        renderText(renderer, game.FONT, textPosition, color, "Acceleration x: " + std::to_string(force.x / player.mass) + ", y: " + std::to_string(force.y / player.mass));
        textPosition.y += 25;
        renderText(renderer, game.FONT, textPosition, color, "Velocity x: " + std::to_string(velocity.x) + ", y: " + std::to_string(velocity.y));
        textPosition.y += 25;
        renderText(renderer, game.FONT, textPosition, color, "Force x: " + std::to_string(force.x) + ", y: " + std::to_string(force.y));      
    }
}

void renderLevel(SDL_Renderer *renderer, entt::registry &reg, GameSettings game, Camera camera, entt::entity levelEntity){
    auto level    = reg.get<Level>(levelEntity);
    auto position = reg.get<Position>(levelEntity);
    auto size     = reg.get<Size>(levelEntity);
    auto color    = reg.get<Color>(levelEntity);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    SDL_FRect fillRect = {(position.x - camera.position.x), (position.y - camera.position.y), (float) size.width, (float) size.height};
    SDL_SetRenderDrawColor(renderer, color.red, color.green, color.blue, color.alpha);
    SDL_RenderFillRectF(renderer, &fillRect);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void renderLevelInfo(SDL_Renderer *renderer, entt::registry &reg, GameSettings game, entt::entity levelEntity){
    SDL_Color color       = { 0x29, 0x2f, 0x36 };
    SDL_Rect textPosition = { 10, game.HEIGHT-35, 0, 0 };
    
    auto level = reg.get<Level>(levelEntity);

    SDL_Rect fillRect = {0, game.HEIGHT-40, 300, game.HEIGHT};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(renderer, &fillRect);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    renderText(renderer, game.FONT, textPosition, color, level.name);
}


void renderPlatforms(SDL_Renderer *renderer, entt::registry &reg, GameSettings game, Camera camera, entt::entity levelEntity){
    const auto view = reg.view<Platform, Size, Position, Color>();
    for(const entt::entity e : view){
        const auto platform = view.get<Platform>(e);

        if(platform.level == levelEntity){
            const auto platformPosition = view.get<Position>(e);
            const auto platformSize     = view.get<Size>(e);
            const auto platformColor    = view.get<Color>(e);
            
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            
            SDL_FRect fillRect = {(platformPosition.x - camera.position.x), (platformPosition.y - camera.position.y), (float) platformSize.width, (float) platformSize.height};
            SDL_SetRenderDrawColor(renderer, platformColor.red, platformColor.green, platformColor.blue, platformColor.alpha);
            SDL_RenderFillRectF(renderer, &fillRect);

            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        }
    }
}

void renderPlayerCollisionBox(SDL_Renderer *renderer, entt::registry &reg, GameSettings game, Camera camera, entt::entity playerEntity){
    const auto player         = reg.get<Player>(playerEntity);
    const auto playerPosition = reg.get<Position>(playerEntity);
    const auto view           = reg.view<CollisionBox, Size, Position>();

    for(const entt::entity e : view){
        const auto collisionBox = view.get<CollisionBox>(e);

        if(collisionBox.entity == playerEntity
            && player.currentAction == collisionBox.action 
            && player.direction == collisionBox.direction
        ){
            const auto collisionBoxPosition = view.get<Position>(e);
            const auto collisionBoxSize     = view.get<Size>(e);

            float boxX = 0.0f;
            float boxY = 0.0f;

            float boxWidth = 0.0f;
            float boxHeight = 0.0f;

            if(collisionBox.action != action::JUMP && (player.isFalling || player.isJumping)){
                // Use jumping collision box as the player animation should still be using the jumping frames.
                const auto jumpSprite = reg.get<JumpSprite>(playerEntity);
                Position jumpSpriteCollisionBoxPosition;
                Size jumpSpriteCollisionBoxSize;

                if(player.direction == direction::LOOKING_RIGHT){
                    jumpSpriteCollisionBoxPosition = reg.get<Position>(jumpSprite.rightCollisionBox);
                    jumpSpriteCollisionBoxSize     = reg.get<Size>(jumpSprite.rightCollisionBox);
                } else {
                    jumpSpriteCollisionBoxPosition = reg.get<Position>(jumpSprite.leftCollisionBox);
                    jumpSpriteCollisionBoxSize     = reg.get<Size>(jumpSprite.leftCollisionBox);
                }                    

                boxX = (playerPosition.x + jumpSpriteCollisionBoxPosition.x) - camera.position.x;
                boxY = (playerPosition.y + jumpSpriteCollisionBoxPosition.y) - camera.position.y;

                boxWidth = (float) jumpSpriteCollisionBoxSize.width;
                boxHeight = (float) jumpSpriteCollisionBoxSize.height;
            
            } else{
                boxX = (playerPosition.x + collisionBoxPosition.x) - camera.position.x;
                boxY = (playerPosition.y + collisionBoxPosition.y) - camera.position.y;

                boxWidth = (float) collisionBoxSize.width;
                boxHeight = (float) collisionBoxSize.height;
            }   

            SDL_FRect cBoxRectF = {boxX, boxY, boxWidth, boxHeight};
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawRectF(renderer, &cBoxRectF);
        }    
    }
}

void renderPlatformCollisionBox(SDL_Renderer *renderer, entt::registry &reg, GameSettings game, Camera camera, entt::entity levelEntity){
    const auto view = reg.view<CollisionBox, Size, Position>();
    for(const entt::entity collisionBoxEntity : view){
        const auto collisionBox         = view.get<CollisionBox>(collisionBoxEntity);

        if(collisionBox.entity == levelEntity){
            const auto collisionBoxPosition = view.get<Position>(collisionBoxEntity);
            const auto collisionBoxSize     = view.get<Size>(collisionBoxEntity);    

            SDL_FRect cBoxRectF = {collisionBoxPosition.x - camera.position.x, collisionBoxPosition.y - camera.position.y, (float) collisionBoxSize.width, (float) collisionBoxSize.height};
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderDrawRectF(renderer, &cBoxRectF);
        }
        
    }
}
