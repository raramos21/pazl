#include "render.hpp"

#include "../Components/player.hpp"

void renderText(SDL_Renderer *renderer, const std::string text, SDL_Rect dest, TTF_Font* gFont){
    SDL_Color fg = { 0, 0xcd, 0x10 };
	SDL_Surface* surf = TTF_RenderText_Solid(gFont, text.c_str(), fg);

	dest.w = surf->w;
	dest.h = surf->h;

	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);

	SDL_RenderCopy(renderer, tex, NULL, &dest);
	SDL_DestroyTexture(tex);
	SDL_FreeSurface(surf);
}

void renderFrameRate(SDL_Renderer *renderer, GameSettings* game) {
    SDL_Rect dest = { 10, 10, 0, 0 };
    renderText(renderer, "Current FPS: " + std::to_string(game->currentFPS), dest, game->gFont);
    dest.y += 24;
    renderText(renderer, "Average FPS: " + std::to_string(game->avgFPS), dest, game->gFont);
    dest.y += 24;
    renderText(renderer, "Current Perf: " + std::to_string(game->currentPerf), dest, game->gFont);
}

void renderPlayer(SDL_Renderer *renderer, entt::registry &reg, GameSettings* game) {
    const auto view = reg.view<Player, Position, IdleAnimationSprite>();
    for(const entt::entity e : view){
        const Player player = view.get<Player>(e);
        const Position position = view.get<Position>(e);
        const IdleAnimationSprite idleSprite = view.get<IdleAnimationSprite>(e);

        SDL_Rect currentClip = idleSprite.idleSpriteClips[game->frames/4];
        SDL_Texture *texture = idleSprite.textureSheet;

        SDL_Rect renderQuad = {(int) position.x, (int) position.y, currentClip.w, currentClip.h};
    
        SDL_RenderCopy(renderer, texture, &currentClip, &renderQuad);        
        SDL_RenderPresent(renderer);

        game->frames += 1;

        if(game->frames / 4 >= idleSprite.total_frames){
            game->frames = 0;
        }
    }
   
}
