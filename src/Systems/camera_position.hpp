#ifndef SYS_CAMERA_POSITION_HPP
#define SYS_CAMERA_POSITION_HPP

#include <entt/entity/registry.hpp>

#include "../Components/components.hpp"

void cameraPosition(entt::registry &, GameSettings, Camera &, entt::entity, entt::entity);

#endif