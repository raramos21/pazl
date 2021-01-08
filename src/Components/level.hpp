#ifndef COMP_LEVEL_HPP
#define COMP_LEVEL_HPP

#include <SDL.h>
#include <vector>
#include <string>

#include "position.hpp"
#include "platform.hpp"
#include "color.hpp"

struct Level{
    std::string name;
    int floor;

    // std::vector<Platform> platforms;
};


#endif