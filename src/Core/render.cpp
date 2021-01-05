#include "render.hpp"

#include "../Components/player.hpp"

void renderText(SDL_Renderer *renderer, TTF_Font* gFont, SDL_Rect dest,  SDL_Color color, const std::string text){
	SDL_Surface* surf = TTF_RenderText_Solid(gFont, text.c_str(), color);

	dest.w = surf->w;
	dest.h = surf->h;

	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);

	SDL_RenderCopy(renderer, tex, NULL, &dest);
	SDL_DestroyTexture(tex);
	SDL_FreeSurface(surf);
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
    SDL_Rect currentClip = sprite.spriteClips[game->FRAMES/total_sprite_frames];
    SDL_Rect renderQuad = {(int) position.x, (int) position.y, currentClip.w+50, currentClip.h+50};
    
    if(player.direction == LOOKING_LEFT){
        SDL_RenderCopyEx(renderer, sprite.textureSheet, &currentClip, &renderQuad, 0, NULL, SDL_FLIP_HORIZONTAL);                 
    } else{
        SDL_RenderCopy(renderer, sprite.textureSheet, &currentClip, &renderQuad);                
    }             
}

void renderPlayer(SDL_Renderer* renderer, entt::registry &reg, GameSettings* game) {
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
        if((player.currentAction == WALK_LEFT || player.currentAction == WALK_RIGHT) ){
            renderSprite(renderer, game, player, position, walkSprite, total_sprite_frames);
        }

        if((player.currentAction == RUN_LEFT || player.currentAction == RUN_RIGHT)){
            renderSprite(renderer, game, player, position, runSprite, total_sprite_frames);
        }

        if(player.currentAction == IDLE){
            renderSprite(renderer, game, player, position, idleSprite, total_sprite_frames);
        }

        if(player.currentAction == JUMP){
            renderSprite(renderer, game, player, position, jumpSprite, total_sprite_frames);
        }

        game->FRAMES += 1;        

        if((game->FRAMES / total_sprite_frames) >= total_sprite_frames){
            game->FRAMES = 0;
        }       
    }   
}

void renderPlayerInfo(SDL_Renderer* renderer, entt::registry &reg, GameSettings* game) {
    const auto view = reg.view<Player, Position, Velocity, Force>();
    for(const entt::entity e : view){
        const auto player   = view.get<Player>(e);
        const auto position = view.get<Position>(e);
        const auto velocity = view.get<Velocity>(e);
        const auto force    = view.get<Force>(e);
        
        SDL_Color color     = { 0x08, 0x4c, 0x61 };
        SDL_Rect dest       = {10, 10, 0, 0};

        std::string lastAction = getPlayerActionString(player.lastAction);
        std::string currentAction = getPlayerActionString(player.currentAction);
        
        renderText(renderer, game->FONT, dest, color, "Current action: " + currentAction);
        dest.y += 25;
        renderText(renderer, game->FONT, dest, color, "Last action: " + lastAction);
        dest.y += 25;
        renderText(renderer, game->FONT, dest, color, "Position x: " + std::to_string(position.x) + ", y: " + std::to_string(position.y));
        dest.y += 25;
        renderText(renderer, game->FONT, dest, color, "Health: " + std::to_string(player.health));
        dest.y += 25;
        renderText(renderer, game->FONT, dest, color, "Mass: " + std::to_string(player.mass));
        dest.y += 25;
        renderText(renderer, game->FONT, dest, color, "Acceleration x: " + std::to_string(force.x / player.mass));
        dest.y += 25;
        renderText(renderer, game->FONT, dest, color, "Velocity x: " + std::to_string(velocity.x) + ", y: " + std::to_string(velocity.y));
        dest.y += 25;
        renderText(renderer, game->FONT, dest, color, "Force x: " + std::to_string(force.x) + ", y: " + std::to_string(force.y));
    }
}


