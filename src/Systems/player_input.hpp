#ifndef SYS_PLAYER_INPUT_HPP
#define SYS_PLAYER_INPUT_HPP

#include <entt/entity/registry.hpp>
#include <SDL_scancode.h>

#include "../Core/input_handler.hpp"

void playerInput(entt::registry &, SDL_Scancode, InputHandler);
void playerDefault(entt::registry &, SDL_Scancode);

#endif