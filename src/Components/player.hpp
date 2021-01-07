#ifndef COMP_PLAYER_HPP
#define COMP_PLAYER_HPP

enum PlayerAction{
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

enum PlayerDirection{
    LOOKING_LEFT,
    LOOKING_RIGHT
};

struct Player{
    int health;
    bool isJumping;
    bool isFalling;
    float mass;
    PlayerAction lastAction;
    PlayerAction currentAction;
    PlayerDirection direction;
};

#endif