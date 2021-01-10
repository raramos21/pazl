#ifndef COMP_PLATFORM_HPP
#define COMP_PLATFORM_HPP

#include <SDL.h>

#include "position.hpp"
#include "size.hpp"
#include <entt/entity/fwd.hpp>

struct Platform{
    entt::entity level;
    SDL_Texture* textureSheet;
};

#endif