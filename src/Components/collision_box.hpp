#ifndef COMP_COLLISION_BOX_HPP
#define COMP_COLLISION_BOX_HPP

#include <entt/entity/fwd.hpp>
#include "player.hpp"

struct CollisionBox{
    entt::entity         entity;
    action::Action       action;
    direction::Direction direction;
};

#endif