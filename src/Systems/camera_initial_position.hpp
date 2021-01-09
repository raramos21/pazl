#ifndef SYS_CAMERA_INITIAL_POSITION_HPP
#define SYS_CAMERA_INITIAL_POSITION_HPP

#include <entt/entity/registry.hpp>

#include "../Components/components.hpp"

void cameraInitialPosition(entt::registry &, GameSettings, Camera &, entt::entity, entt::entity);

#endif
