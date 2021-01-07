#include "factories.hpp"

#include "../Components/player.hpp"
#include "../Util/sdl_check.hpp"
#include "../Core/load.hpp"

#include <entt/entity/registry.hpp>


entt::entity makePlayer(SDL_Renderer * renderer, entt::registry &reg, GameSettings* game){
    const entt::entity e = reg.create();
    auto &player = reg.emplace<Player>(e);
    auto &position = reg.emplace<Position>(e);
    auto &velocity = reg.emplace<Velocity>(e);
    auto &force = reg.emplace<Force>(e);
    player.health = 100;
    player.mass = 1;
    player.isJumping = false;
    player.isFalling = false;
    player.currentAction = IDLE;
    velocity.x = 0;
    velocity.y = 0;
    force.x = 0;
    force.y = 0;

    auto &idleSprite = reg.emplace<IdleSprite>(e);
    SDL_CHECK(loadSpriteFromFile(renderer, idleSprite, "assets/player/Woodcutter_idle.png"));

    idleSprite.total_frames = 8;    
    SDL_Rect idle1{ 0, 0, 40, 45};
    SDL_Rect idle2{ 45, 0, 40, 45};
    SDL_Rect idle3{ 90, 0, 40, 45};
    SDL_Rect idle4{ 135, 0, 40, 45};

    idleSprite.spriteClips.push_back(idle1);
    idleSprite.spriteClips.push_back(idle1);
    idleSprite.spriteClips.push_back(idle2);
    idleSprite.spriteClips.push_back(idle2);
    idleSprite.spriteClips.push_back(idle3);
    idleSprite.spriteClips.push_back(idle3);
    idleSprite.spriteClips.push_back(idle4);
    idleSprite.spriteClips.push_back(idle4);

    position.x = (game->WIDTH  - idleSprite.spriteClips[0].w)/2;
    position.y = 350.0f;// (game->HEIGHT - idleSprite.spriteClips[0].h)/2;

    auto &runSprite = reg.emplace<RunSprite>(e);
    SDL_CHECK(loadSpriteFromFile(renderer, runSprite, "assets/player/Woodcutter_run.png"));

    runSprite.total_frames = 6;    
    SDL_Rect run1{ 0, 0, 40, 45};
    SDL_Rect run2{ 45, 0, 40, 45};
    SDL_Rect run3{ 90, 0, 40, 45};
    SDL_Rect run4{ 135, 0, 40, 45};
    SDL_Rect run5{ 180, 0, 40, 45};
    SDL_Rect run6{ 225, 0, 40, 45};

    runSprite.spriteClips.push_back(run1);
    // runSprite.spriteClips.push_back(run1);
    runSprite.spriteClips.push_back(run2);
    // runSprite.spriteClips.push_back(run2);
    runSprite.spriteClips.push_back(run3);
    // runSprite.spriteClips.push_back(run3);
    runSprite.spriteClips.push_back(run4);
    // runSprite.spriteClips.push_back(run4);
    runSprite.spriteClips.push_back(run5);
    // runSprite.spriteClips.push_back(run5);
    runSprite.spriteClips.push_back(run6);
    // runSprite.spriteClips.push_back(run6);
    
    auto &walkSprite = reg.emplace<WalkSprite>(e);
    SDL_CHECK(loadSpriteFromFile(renderer, walkSprite, "assets/player/Woodcutter_walk.png"));

    walkSprite.total_frames = 12;    
    SDL_Rect walk1{ 0, 0, 40, 45};
    SDL_Rect walk2{ 45, 0, 40, 45};
    SDL_Rect walk3{ 90, 0, 40, 45};
    SDL_Rect walk4{ 135, 0, 40, 45};
    SDL_Rect walk5{ 180, 0, 40, 45};
    SDL_Rect walk6{ 225, 0, 40, 45};

    walkSprite.spriteClips.push_back(walk1);
    walkSprite.spriteClips.push_back(walk1);
    walkSprite.spriteClips.push_back(walk2);
    walkSprite.spriteClips.push_back(walk2);
    walkSprite.spriteClips.push_back(walk3);
    walkSprite.spriteClips.push_back(walk3);
    walkSprite.spriteClips.push_back(walk4);
    walkSprite.spriteClips.push_back(walk4);
    walkSprite.spriteClips.push_back(walk5);
    walkSprite.spriteClips.push_back(walk5);
    walkSprite.spriteClips.push_back(walk6);
    walkSprite.spriteClips.push_back(walk6);


    auto &jumpSprite = reg.emplace<JumpSprite>(e);
    SDL_CHECK(loadSpriteFromFile(renderer, jumpSprite, "assets/player/Woodcutter_jump.png"));

    jumpSprite.total_frames = 8;    
    SDL_Rect jump1{ 0, 0, 40, 45};
    SDL_Rect jump2{ 45, 0, 40, 45};
    SDL_Rect jump3{ 90, 0, 40, 45};
    SDL_Rect jump4{ 135, 0, 40, 45};
    SDL_Rect jump5{ 180, 0, 40, 45};
    SDL_Rect jump6{ 225, 0, 40, 45};

    jumpSprite.spriteClips.push_back(jump1);
    // jumpSprite.spriteClips.push_back(jump1);
    jumpSprite.spriteClips.push_back(jump2);
    // jumpSprite.spriteClips.push_back(jump2);
    jumpSprite.spriteClips.push_back(jump3);
    jumpSprite.spriteClips.push_back(jump3);
    jumpSprite.spriteClips.push_back(jump4);
    jumpSprite.spriteClips.push_back(jump4);
    jumpSprite.spriteClips.push_back(jump5);
    // jumpSprite.spriteClips.push_back(jump5);
    jumpSprite.spriteClips.push_back(jump6);
    // jumpSprite.spriteClips.push_back(jump6);
    
    
    return e;
}
