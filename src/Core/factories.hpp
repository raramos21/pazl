#ifndef CORE_FACTORIES_HPP
#define CORE_FACTORIES_HPP

#include <SDL_render.h>
#include <entt/entity/fwd.hpp>

#include "../Components/game_settings.hpp"

entt::entity makePlayer(SDL_Renderer*, entt::registry &, GameSettings*);

#endif