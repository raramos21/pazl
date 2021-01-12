#ifndef COMP_PLAYER_HPP
#define COMP_PLAYER_HPP

namespace action{
    enum Action{
        WALK,
        RUN,
        JUMP,    
        IDLE,
        ATTACK,
        NONE
    };
}

namespace direction{
    enum Direction{
        LOOKING_LEFT,
        LOOKING_RIGHT,
        NONE
    };
}

struct Player{
    int health;
    bool isJumping{false};
    bool isFalling{false};
    bool isColliding{false};
    bool isGoingToCollide{false};
    float mass;

    action::Action       lastAction;
    action::Action       currentAction;
    direction::Direction direction;
};

#endif