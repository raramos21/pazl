#ifndef CORE_FACTORIES_HPP
#define CORE_FACTORIES_HPP

#include <SDL_render.h>
#include <entt/entity/fwd.hpp>
#include <string>

#include "../Components/components.hpp"

entt::entity makePlayer(SDL_Renderer*, entt::registry &, GameSettings);
entt::entity makeLevel(entt::registry &, GameSettings, float, float, Color, std::string);
entt::entity makePlatform(entt::registry &, GameSettings, entt::entity, Position, Size, Color);
entt::entity makeCollisionBox(entt::registry &, entt::entity, Position, Size, action::Action, direction::Direction);

#endif