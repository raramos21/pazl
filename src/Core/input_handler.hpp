#ifndef CORE_INPUT_HANDLER_HPP
#define CORE_INPUT_HANDLER_HPP

#include <SDL_scancode.h>

class InputHandler{

    public:
    enum State{
        PRESSED,
        RELEASED
    };
    State inputState[];

    void handleInputEvent(SDL_Scancode);

    bool isPressed(SDL_Scancode);

    bool isReleased(SDL_Scancode);
};

#endif