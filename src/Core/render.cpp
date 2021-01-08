#include "render.hpp"

#include "../Components/components.hpp"

void renderText(SDL_Renderer *renderer, TTF_Font* gFont, SDL_Rect dest,  SDL_Color color, const std::string text){
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

void renderFrameRate(SDL_Renderer *renderer, GameSettings* game) {
    SDL_Rect dest = { 10, 10, 0, 0 };
    SDL_Color color = { 0, 0xcd, 0x10 };
    renderText(renderer, game->FONT, dest, color, "Current FPS: " + std::to_string(game->CURRENT_FPS));
    dest.y += 24;
    renderText(renderer, game->FONT, dest, color, "Average FPS: " + std::to_string(game->AVERAGE_FPS));
    dest.y += 24;
    renderText(renderer, game->FONT, dest, color, "Current Perf: " + std::to_string(game->CURRENT_PERF));
}

std::string getPlayerActionString(PlayerAction action){
    std::string actionString = "NONE";
    switch(action){
        case WALK_RIGHT:
            actionString = "WALK_RIGHT";
            break;
        case WALK_LEFT:
            actionString = "WALK_LEFT";
            break;
        case JUMP:
            actionString = "JUMP";
            break;
        case RUN_RIGHT:
            actionString = "RUN_RIGHT";
            break;
        case RUN_LEFT:
            actionString = "RUN_LEFT";
            break;
        case IDLE:
            actionString = "IDLE";
            break;
        case ATTACK:
            actionString = "ATTACK";
            break;
        case RESET:
            actionString = "RESET";
            break;
        default:
            actionString = "NONE";
            break;
    }
    return actionString;
}

void renderSprite(SDL_Renderer* renderer, GameSettings* game, Player player, Position position, Sprite sprite, int & total_sprite_frames){
    total_sprite_frames = sprite.total_frames;
    // int clip = game->FRAMES / total_sprite_frames;
    int clip = game->FRAMES;
    SDL_Rect currentClip = sprite.spriteClips[clip];
    SDL_Rect renderQuad = {(int) position.x, (int) position.y, sprite.size.width, sprite.size.height};

    // Render red rectangle around sprite.
    SDL_Rect fillRect = {(int) position.x, (int) position.y, sprite.size.width, sprite.size.height};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
    SDL_RenderDrawRect(renderer, &fillRect);

    // Render current clip number.
    SDL_Color color       = { 0xff, 0xff, 0xff };
    SDL_Rect textPosition = {(int) position.x + 5, (int) position.y + 2, 0, 0};
    renderText(renderer, game->FONT, textPosition, color, std::to_string(clip));    
    
    if(player.direction == LOOKING_LEFT){
        SDL_RenderCopyEx(renderer, sprite.textureSheet, &currentClip, &renderQuad, 0, NULL, SDL_FLIP_HORIZONTAL);                 
    } else{
        SDL_RenderCopyEx(renderer, sprite.textureSheet, &currentClip, &renderQuad, 0, NULL, SDL_FLIP_NONE);                
    }             
}

void renderPlayer(SDL_Renderer* renderer, entt::registry &reg, GameSettings* game){
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
        // if(player.currentAction != player.lastAction){
        //     game->FRAMES = 0;
        // }
        
        if(!player.isJumping){
            if(player.currentAction == WALK_LEFT || player.currentAction == WALK_RIGHT){      
                renderSprite(renderer, game, player, position, walkSprite, total_sprite_frames);
            }

            if(player.currentAction == RUN_LEFT || player.currentAction == RUN_RIGHT){
                renderSprite(renderer, game, player, position, runSprite, total_sprite_frames);
            }

            if(player.currentAction == IDLE || player.currentAction == JUMP){
                renderSprite(renderer, game, player, position, idleSprite, total_sprite_frames);
            }
        } else {
            renderSprite(renderer, game, player, position, jumpSprite, total_sprite_frames);
        }

        game->FRAMES += 1;        

        // if((game->FRAMES / total_sprite_frames) >= total_sprite_frames){
        //     game->FRAMES = 0;
        // }      

        if(game->FRAMES >= total_sprite_frames){
            game->FRAMES = 0;
        } 
    }   
}



void renderPlayerInfo(SDL_Renderer* renderer, entt::registry &reg, GameSettings game){
    const auto view = reg.view<Player, Position, Velocity, Force>();
    for(const entt::entity e : view){
        const auto player   = view.get<Player>(e);
        const auto position = view.get<Position>(e);
        const auto velocity = view.get<Velocity>(e);
        const auto force    = view.get<Force>(e);
        
        std::string lastAction = getPlayerActionString(player.lastAction);
        std::string currentAction = getPlayerActionString(player.currentAction);

        SDL_Rect fillRect = {0, 0, 450, 220};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(renderer, &fillRect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

        SDL_Color color       = { 0x29, 0x2f, 0x36 };
        SDL_Rect textPosition = {10, 10, 0, 0};
        
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
        renderText(renderer, game.FONT, textPosition, color, "Acceleration x: " + std::to_string(force.x / player.mass));
        textPosition.y += 25;
        renderText(renderer, game.FONT, textPosition, color, "Velocity x: " + std::to_string(velocity.x) + ", y: " + std::to_string(velocity.y));
        textPosition.y += 25;
        renderText(renderer, game.FONT, textPosition, color, "Force x: " + std::to_string(force.x) + ", y: " + std::to_string(force.y));      
    }
}

void renderLevel(SDL_Renderer* renderer, entt::registry &reg, GameSettings game, entt::entity e){
    auto level    = reg.get<Level>(e);
    auto position = reg.get<Position>(e);
    auto size     = reg.get<Size>(e);
    auto color    = reg.get<Color>(e);

    SDL_Rect fillRect = {(int) position.x, (int) position.y, size.width, size.height};
    SDL_SetRenderDrawColor(renderer, color.red, color.green, color.blue, color.alpha);
    SDL_RenderFillRect(renderer, &fillRect);
}

void renderLevelInfo(SDL_Renderer* renderer, entt::registry &reg, GameSettings game, entt::entity e){
    SDL_Color color       = { 0x29, 0x2f, 0x36 };
    SDL_Rect textPosition = { 10, game.HEIGHT-35, 0, 0 };
    
    auto level = reg.get<Level>(e);

    SDL_Rect fillRect = {0, game.HEIGHT-40, 300, game.HEIGHT};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(renderer, &fillRect);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);


    renderText(renderer, game.FONT, textPosition, color, level.name);
}


