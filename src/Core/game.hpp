#ifndef CORE_GAME_HPP
#define CORE_GAME_HPP

// #include <SDL_Windo?
#include <SDL_render.h>
#include <SDL_scancode.h>
#include <entt/entity/registry.hpp>
#include "../Components/game_settings.hpp"

void gameInit(SDL_Renderer*, entt::registry &, GameSettings*);
void gameInput(entt::registry &,  SDL_Scancode);
void gameDefaultInput(entt::registry &);
void gameLogic(entt::registry &, double, float);
void gameRender(SDL_Renderer*, entt::registry &, GameSettings*);
void gameQuit(SDL_Window*, SDL_Renderer*, entt::registry &, GameSettings*);

#endif