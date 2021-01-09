#ifndef SYS_SET_CAMERA_HPP
#define SYS_SET_CAMERA_HPP

#include <SDL_render.h>
#include <entt/entity/registry.hpp>

#include "../Components/components.hpp"

void setCamera(SDL_Renderer *, entt::registry &, GameSettings, Camera &, entt::entity, entt::entity);

#endif