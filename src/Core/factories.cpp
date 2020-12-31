#include "factories.hpp"

#include "../Components/player.hpp"
#include "../Util/sdl_check.hpp"
#include "../Core/load.hpp"

#include <entt/entity/registry.hpp>


entt::entity makePlayer(SDL_Renderer * renderer, entt::registry &reg, GameSettings* game){
    const entt::entity e = reg.create();
    reg.emplace<Player>(e);

    auto &position = reg.emplace<Position>(e);  

    auto &idleSprite = reg.emplace<IdleAnimationSprite>(e);
    SDL_CHECK(loadSpriteFromFile(renderer, idleSprite, "assets/1 Woodcutter/Woodcutter_idle.png"));

    idleSprite.total_frames = 4;    
    SDL_Rect c1{ 0, 16, 32, 28};
    SDL_Rect c2{ 48, 16, 32, 28};
    SDL_Rect c3{ 95, 16, 32, 28};
    SDL_Rect c4{ 143, 16, 32, 28};

    idleSprite.idleSpriteClips.push_back(c1);
    idleSprite.idleSpriteClips.push_back(c2);
    idleSprite.idleSpriteClips.push_back(c3);
    idleSprite.idleSpriteClips.push_back(c4);

    position.x = (game->width  - idleSprite.idleSpriteClips[0].w)/2 + 50;
    position.y = (game->height - idleSprite.idleSpriteClips[0].h)/2 - 50;
    // auto &AttackSprite = reg.emplace<AttackAnimationSprite>(e);
    
    return e;
}
