#ifndef UTIL_CLAMP_HPP
#define UTIL_CLAMP_HPP

inline
float clamp(float value, float min, float max){
    if(value < min){
        value = min;
    }

    if(value > max){
        value = max;
    }
}

#endif