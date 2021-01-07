#ifndef COMP_PLAYER_HPP
#define COMP_PLAYER_HPP

#include <vector>
#include <SDL.h>

enum PlayerAction {
    WALK_RIGHT,
    WALK_LEFT,
    RUN_RIGHT,
    RUN_LEFT,
    JUMP,
    RUN,
    IDLE,
    ATTACK,
    RESET
};

enum PlayerDirection {
    LOOKING_LEFT,
    LOOKING_RIGHT
};

struct Player {
    int health;
    bool isJumping;
    bool isFalling;
    float mass;
    PlayerAction lastAction;
    PlayerAction currentAction;
    PlayerDirection direction;
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

struct IdleSprite   : public Sprite {};
struct WalkSprite   : public Sprite {};
struct JumpSprite   : public Sprite {};
struct RunSprite    : public Sprite {};
struct DieSprite    : public Sprite {};
struct AttackSprite : public Sprite {};


#endif