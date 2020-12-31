#ifndef CORE_LOAD_HPP
#define CORE_LOAD_HPP

#include <SDL_render.h>
#include <entt/entity/fwd.hpp>
#include <string>

#include "../Components/player.hpp"
#include "../Components/game_settings.hpp"

bool loadFramerateFont(GameSettings* );
bool loadSpriteFromFile(SDL_Renderer *, IdleAnimationSprite &, std::string );

#endif