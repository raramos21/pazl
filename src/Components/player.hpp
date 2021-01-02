#ifndef COMP_PLAYER_HPP
#define COMP_PLAYER_HPP

#include <vector>
#include <SDL.h>

enum PlayerAction {
    MOVE_RIGHT,
    MOVE_LEFT,
    JUMP,
    RUN,
    IDLE,
    ATTACK
};

struct Player {
    int health;
    float mass;
    PlayerAction lastAction;
    PlayerAction currentAction;
};

struct Position {
    float x;
    float y;
};

struct Velocity {
    float x;
    float y;
};

struct Force {
    float x;
    float y;
};

// struct State {
//     float x;
//     float vx;
//     float y;
//     float vy;
// };

// struct Derivative{
//     float dx;
//     float dvx;
//     float dy;
//     float dvy;
// };

struct Sprite {
    int total_frames;
    int width;
    int height;
    SDL_Texture* textureSheet;
    std::vector<SDL_Rect> spriteClips;
};

struct IdleSprite : public Sprite {};
struct RunSprite : public Sprite {};
struct AttackSprite : public Sprite {};
struct DieSprite : public Sprite {};

#endif