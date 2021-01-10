#ifndef SYS_RESET_PLAYER_POSITION_HPP
#define SYS_RESET_PLAYER_POSITION_HPP

#include <SDL_scancode.h>
#include <entt/entity/registry.hpp>

void resetPlayerPosition(entt::registry &, SDL_Scancode, entt::entity, entt::entity);

#endif