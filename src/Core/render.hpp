#ifndef CORE_RENDER_HPP
#define CORE_RENDER_HPP

#include <SDL_render.h>
#include <SDL_rect.h>
#include <entt/entity/registry.hpp>
#include <string>
#include "../Components/game_settings.hpp"

void renderText(SDL_Renderer *, TTF_Font*, SDL_Rect,  SDL_Color, const std::string);
void renderFrameRate(SDL_Renderer*, GameSettings* );
void renderPlayer(SDL_Renderer *, entt::registry &, GameSettings*);

#endif
