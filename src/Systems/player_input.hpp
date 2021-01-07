#ifndef SYS_PLAYER_INPUT_HPP
#define SYS_PLAYER_INPUT_HPP

#include <entt/entity/registry.hpp>
#include <SDL_scancode.h>

void playerInput(entt::registry &, SDL_Scancode, const Uint8*);
void playerDefault(entt::registry &, SDL_Scancode);

#endif