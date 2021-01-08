#ifndef CORE_GAME_HPP
#define CORE_GAME_HPP

// #include <SDL_Windo?
#include <SDL_render.h>
#include <SDL_scancode.h>
#include <entt/entity/registry.hpp>
#include "../Components/game_settings.hpp"

GameSettings gameInit();
void gameCreateEntities(SDL_Renderer*);
void gameInput(SDL_Scancode, const Uint8*);
void gameChangeLevels(SDL_Scancode);
void gameDefaultInput(SDL_Scancode);
void gameLogic(double, float);
void gameRender(SDL_Renderer*);
void gameQuit(SDL_Window*, SDL_Renderer*);

#endif