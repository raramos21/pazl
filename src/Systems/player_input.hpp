#ifndef SYS_PLAYER_INPUT_HPP
#define SYS_PLAYER_INPUT_HPP

#include <entt/entity/registry.hpp>
#include <SDL_scancode.h>

void playerInput(entt::registry &, SDL_Scancode);
void playerDefault(entt::registry &);

#endif