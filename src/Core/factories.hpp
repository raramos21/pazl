#ifndef CORE_FACTORIES_HPP
#define CORE_FACTORIES_HPP

#include <SDL_render.h>
#include <entt/entity/fwd.hpp>
#include <string>

#include "../Components/components.hpp"

entt::entity makePlayer(SDL_Renderer*, entt::registry &, GameSettings*);
entt::entity makeLevel(SDL_Renderer*, entt::registry &, GameSettings*, float, float, Color, std::string);

#endif