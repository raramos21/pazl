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
        case MOVE_RIGHT:
            actionString = "MOVE_RIGHT";
            break;
        case MOVE_LEFT:
            actionString = "MOVE_LEFT";
            break;
        case JUMP:
            actionString = "JUMP";
            break;
        case RUN:
            actionString = "RUN";
            break;
        case IDLE:
            actionString = "IDLE";
            break;
        case ATTACK:
            actionString = "ATTACK";
            break;
        default:
            actionString = "NONE";
            break;
    }
    return actionString;
}

void renderPlayer(SDL_Renderer *renderer, entt::registry &reg, GameSettings* game) {
    const auto view = reg.view<Player, Position, Velocity, Force, IdleSprite, RunSprite>();
    for(const entt::entity e : view){
        const auto player   = view.get<Player>(e);
        const auto position = view.get<Position>(e);
        const auto velocity = view.get<Velocity>(e);
        const auto force    = view.get<Force>(e);

        // if(game->FRAMES %2 != 0 && game->FRAMES != 0){
        //     game->FRAMES -= 1;
        // }

        int total_sprite_frames = 1;
        if((player.currentAction == MOVE_LEFT || player.currentAction == MOVE_RIGHT) ){
            const auto runSprite = view.get<RunSprite>(e);
            total_sprite_frames = runSprite.total_frames;
            SDL_Rect currentClip = runSprite.spriteClips[game->FRAMES/total_sprite_frames];
            SDL_Rect renderQuad = {(int) position.x, (int) position.y, currentClip.w+50, currentClip.h+50};
            
            if(player.currentAction == MOVE_RIGHT){
                SDL_RenderCopy(renderer, runSprite.textureSheet, &currentClip, &renderQuad);                
            } else{
                SDL_RenderCopyEx(renderer, runSprite.textureSheet, &currentClip, &renderQuad, 0, NULL, SDL_FLIP_HORIZONTAL); 
            }             
        }        

        if(player.currentAction == IDLE){
            const auto idleSprite = view.get<IdleSprite>(e);
            total_sprite_frames = idleSprite.total_frames;
            SDL_Rect currentClip = idleSprite.spriteClips[game->FRAMES/total_sprite_frames];
            SDL_Rect renderQuad = {(int) position.x, (int) position.y, currentClip.w+50, currentClip.h+50};
        
            SDL_RenderCopy(renderer, idleSprite.textureSheet, &currentClip, &renderQuad);
        }

        // if(game->FRAMES %2 == 0){
            game->FRAMES += 1;
        // }
        

        if((game->FRAMES / total_sprite_frames) >= total_sprite_frames){
            game->FRAMES = 0;
        }       
        
        SDL_Color color = { 0x08, 0x4c, 0x61 };
        SDL_Rect dest = {10, 10, 0, 0};

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


