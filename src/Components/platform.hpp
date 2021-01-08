#ifndef COMP_PLATFORM_HPP
#define COMP_PLATFORM_HPP

#include <SDL.h>

#include "position.hpp"
#include "size.hpp"

struct Platform{
    Position position;
    Size size;

    SDL_Texture* textureSheet;
};

#endif