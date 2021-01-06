#include "input_handler.hpp"

void InputHandler::handleInputEvent(SDL_Scancode scancode){
    if(inputState[scancode] == PRESSED){
        inputState[scancode] = RELEASED;
    }
    inputState[scancode] = PRESSED;
}

bool InputHandler::isPressed(SDL_Scancode scancode){
    return (inputState[scancode] == PRESSED);
}

bool InputHandler::isReleased(SDL_Scancode scancode){
    return (inputState[scancode] == RELEASED);
}