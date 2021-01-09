#ifndef SYS_CHANGE_LEVELS_HPP
#define SYS_CHANGE_LEVELS_HPP

#include <SDL_scancode.h>
#include <entt/entity/registry.hpp>
#include <vector>
#include "../Components/components.hpp"

void changeLevels(entt::registry &, SDL_Scancode, GameSettings, Camera &, entt::entity , int & , std::vector<entt::entity>);

#endif