#include "load.hpp"

#include "../Util/sdl_check.hpp"

#include <SDL_image.h>
#include <iostream>

bool loadFramerateFont(GameSettings* game) { 
    bool success = true;
    const char *font = "assets/Roboto-Regular.ttf";

    game->FONT = TTF_OpenFont(font, 24);
	if ( game->FONT == NULL ) {
		printf("Error loading font: %s\n", TTF_GetError());
		success = false;
	}

    return success;
}

bool loadSpriteFromFile(SDL_Renderer *renderer, Sprite &sprite, std::string path){
    SDL_Texture* newTexture    = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());

    if(loadedSurface == NULL){
        printf("Unable to load image %s! SDL_image error: %s\n", path.c_str(), IMG_GetError());
    } else {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

        if(newTexture == NULL){
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        } else {    
            sprite.width  = loadedSurface->w;
            sprite.height = loadedSurface->h;
        }
        SDL_FreeSurface(loadedSurface);
    }
    
    sprite.textureSheet = newTexture;
    return sprite.textureSheet != NULL;
}
