#ifndef COMP_SPRITE_HPP
#define COMP_SPRITE_HPP

#include <vector>
#include <SDL.h>

#include "size.hpp"

struct Sprite{
    int total_frames;
    Size size;
    SDL_Texture* textureSheet;
    std::vector<SDL_Rect> spriteClips;
};

struct IdleSprite   : public Sprite {};
struct WalkSprite   : public Sprite {};
struct JumpSprite   : public Sprite {};
struct RunSprite    : public Sprite {};
struct DieSprite    : public Sprite {};
struct AttackSprite : public Sprite {};

#endif