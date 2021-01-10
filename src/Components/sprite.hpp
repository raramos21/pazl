#ifndef COMP_SPRITE_HPP
#define COMP_SPRITE_HPP

#include <vector>
#include <SDL.h>
#include <entt/entity/fwd.hpp>

#include "size.hpp"

struct Sprite{
    int total_frames;
    Size size;
    Size textureSheetSize;
    SDL_Texture* textureSheet;
    entt::entity collisionBox;
    std::vector<SDL_Rect> spriteClips;
};

struct IdleSprite   : public Sprite {};
struct WalkSprite   : public Sprite {};
struct JumpSprite   : public Sprite {};
struct RunSprite    : public Sprite {};
struct DieSprite    : public Sprite {};
struct AttackSprite : public Sprite {};

#endif