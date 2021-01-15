#ifndef UTIL_CLAMP_HPP
#define UTIL_CLAMP_HPP

#include "../Components/position.hpp"

inline
float clampX(float value, float min, float max){
    if(value < min){
        value = min;
    }

    if(value > max){
        value = max;
    }

    return value;
}

inline
float clampY(float value, float min, float max){
    if(value > min){
        value = min;
    }

    if(value < max){
        value = max;
    }

    return value;
}


inline
void clamp(Position *p, float xMin, float xMax, float yMin, float yMax){
    p->x = clampX(p->x, xMin, xMax);
    p->y = clampY(p->y, yMin, yMax); 
}

#endif