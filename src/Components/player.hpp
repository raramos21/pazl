#ifndef COMP_PLAYER_HPP
#define COMP_PLAYER_HPP

#include <vector>
#include <SDL.h>

struct Player {
    int health;
};

struct Position {
    float x;
    float y;
};

struct Sprite {
    int total_frames;
    int width;
    int height;
    SDL_Texture* textureSheet;
    std::vector<SDL_Rect> idleSpriteClips;
};

struct IdleAnimationSprite : public Sprite {};
struct AttackAnimationSprite : public Sprite {};
struct DieAnimationSprite : public Sprite {};

#endif