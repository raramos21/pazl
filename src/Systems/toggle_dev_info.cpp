#include "toggle_dev_info.hpp"

void toggleDevInfo(SDL_Scancode scancode, bool & showDevInfo){
    if(scancode == SDL_SCANCODE_I){
        showDevInfo = !showDevInfo;
    }
}