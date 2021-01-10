#include "factories.hpp"

#include "../Components/components.hpp"
#include "../Util/sdl_check.hpp"
#include "../Core/load.hpp"

#include <entt/entity/registry.hpp>

const Size DEFAULT_SPRITE{40, 45};
const Size DEFAULT_PLATFORM{50, 100};

const int SPRITE_SCALE_FACTOR = 2;

entt::entity makeCollisionBox(entt::registry &reg, entt::entity playerEntity, Position position, Size size, PlayerAction action, PlayerDirection direction){    
    const entt::entity collisionEntity = reg.create();

    auto &collisionBox         = reg.emplace<CollisionBox>(collisionEntity);
    auto &collisionBoxPosition = reg.emplace<Position>(collisionEntity);
    auto &collisionBoxSize     = reg.emplace<Size>(collisionEntity);

    collisionBox.player    = playerEntity;
    collisionBox.action    = action;
    collisionBox.direction = direction;
    collisionBoxPosition   = position;
    collisionBoxSize       = size;
    
    return collisionEntity;
}

entt::entity makePlayer(SDL_Renderer *renderer, entt::registry &reg, GameSettings game){
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
    SDL_CHECK(loadSpriteFromFile(renderer, idleSprite, "assets/player/Woodcutter_idle_red.png"));

    idleSprite.total_frames = 32;    
    SDL_Rect idle1{ 0, 0, 40, 45};
    SDL_Rect idle2{ 45, 0, 40, 45};
    SDL_Rect idle3{ 90, 0, 40, 45};
    SDL_Rect idle4{ 135, 0, 40, 45};

    idleSprite.spriteClips.push_back(idle1);
    idleSprite.spriteClips.push_back(idle1);
    idleSprite.spriteClips.push_back(idle1);
    idleSprite.spriteClips.push_back(idle1);
    idleSprite.spriteClips.push_back(idle1);
    idleSprite.spriteClips.push_back(idle1);
    idleSprite.spriteClips.push_back(idle1);
    idleSprite.spriteClips.push_back(idle1);
    idleSprite.spriteClips.push_back(idle2);
    idleSprite.spriteClips.push_back(idle2);
    idleSprite.spriteClips.push_back(idle2);
    idleSprite.spriteClips.push_back(idle2);
    idleSprite.spriteClips.push_back(idle2);
    idleSprite.spriteClips.push_back(idle2);
    idleSprite.spriteClips.push_back(idle2);
    idleSprite.spriteClips.push_back(idle2);
    idleSprite.spriteClips.push_back(idle3);
    idleSprite.spriteClips.push_back(idle3);
    idleSprite.spriteClips.push_back(idle3);
    idleSprite.spriteClips.push_back(idle3);
    idleSprite.spriteClips.push_back(idle3);
    idleSprite.spriteClips.push_back(idle3);
    idleSprite.spriteClips.push_back(idle3);
    idleSprite.spriteClips.push_back(idle3);
    idleSprite.spriteClips.push_back(idle4);
    idleSprite.spriteClips.push_back(idle4);
    idleSprite.spriteClips.push_back(idle4);
    idleSprite.spriteClips.push_back(idle4);
    idleSprite.spriteClips.push_back(idle4);
    idleSprite.spriteClips.push_back(idle4);
    idleSprite.spriteClips.push_back(idle4);
    idleSprite.spriteClips.push_back(idle4);
    
    idleSprite.size.width  = DEFAULT_SPRITE.width * SPRITE_SCALE_FACTOR;
    idleSprite.size.height = DEFAULT_SPRITE.height * SPRITE_SCALE_FACTOR;

    position.x = (game.WIDTH  - idleSprite.spriteClips[0].w)/2;
    position.y = (game.HEIGHT - idleSprite.spriteClips[0].h)/2;

    auto &runSprite = reg.emplace<RunSprite>(e);
    SDL_CHECK(loadSpriteFromFile(renderer, runSprite, "assets/player/Woodcutter_run_red.png"));

    runSprite.total_frames = 22;    
    SDL_Rect run1{ 0, 0, 40, 45};
    SDL_Rect run2{ 45, 0, 40, 45};
    SDL_Rect run3{ 90, 0, 40, 45};
    SDL_Rect run4{ 135, 0, 40, 45};
    SDL_Rect run5{ 180, 0, 40, 45};
    SDL_Rect run6{ 225, 0, 40, 45};

    runSprite.spriteClips.push_back(run1);
    runSprite.spriteClips.push_back(run1);
    runSprite.spriteClips.push_back(run1);
    runSprite.spriteClips.push_back(run1);
    runSprite.spriteClips.push_back(run2);
    runSprite.spriteClips.push_back(run2);
    runSprite.spriteClips.push_back(run2);
    runSprite.spriteClips.push_back(run2);
    runSprite.spriteClips.push_back(run3);
    runSprite.spriteClips.push_back(run3);
    runSprite.spriteClips.push_back(run3);
    runSprite.spriteClips.push_back(run3);
    runSprite.spriteClips.push_back(run4);
    runSprite.spriteClips.push_back(run4);
    runSprite.spriteClips.push_back(run4);
    runSprite.spriteClips.push_back(run4);
    runSprite.spriteClips.push_back(run5);
    runSprite.spriteClips.push_back(run5);
    runSprite.spriteClips.push_back(run5);
    runSprite.spriteClips.push_back(run5);
    runSprite.spriteClips.push_back(run6);
    runSprite.spriteClips.push_back(run6);
    runSprite.spriteClips.push_back(run6);
    runSprite.spriteClips.push_back(run6);

    runSprite.size.width  = DEFAULT_SPRITE.width * SPRITE_SCALE_FACTOR;
    runSprite.size.height = DEFAULT_SPRITE.height * SPRITE_SCALE_FACTOR;
    
    auto &walkSprite = reg.emplace<WalkSprite>(e);
    SDL_CHECK(loadSpriteFromFile(renderer, walkSprite, "assets/player/Woodcutter_walk_red.png"));

    walkSprite.total_frames = 30;    
    SDL_Rect walk1{ 0, 0, 40, 45};
    SDL_Rect walk2{ 45, 0, 40, 45};
    SDL_Rect walk3{ 90, 0, 40, 45};
    SDL_Rect walk4{ 135, 0, 40, 45};
    SDL_Rect walk5{ 180, 0, 40, 45};
    SDL_Rect walk6{ 225, 0, 40, 45};

    walkSprite.spriteClips.push_back(walk1);
    walkSprite.spriteClips.push_back(walk1);
    walkSprite.spriteClips.push_back(walk1);
    walkSprite.spriteClips.push_back(walk1);
    walkSprite.spriteClips.push_back(walk1);
    walkSprite.spriteClips.push_back(walk2);
    walkSprite.spriteClips.push_back(walk2);
    walkSprite.spriteClips.push_back(walk2);
    walkSprite.spriteClips.push_back(walk2);
    walkSprite.spriteClips.push_back(walk2);
    walkSprite.spriteClips.push_back(walk3);
    walkSprite.spriteClips.push_back(walk3);
    walkSprite.spriteClips.push_back(walk3);
    walkSprite.spriteClips.push_back(walk3);
    walkSprite.spriteClips.push_back(walk3);
    walkSprite.spriteClips.push_back(walk4);
    walkSprite.spriteClips.push_back(walk4);
    walkSprite.spriteClips.push_back(walk4);
    walkSprite.spriteClips.push_back(walk4);
    walkSprite.spriteClips.push_back(walk4);
    walkSprite.spriteClips.push_back(walk5);
    walkSprite.spriteClips.push_back(walk5);
    walkSprite.spriteClips.push_back(walk5);
    walkSprite.spriteClips.push_back(walk5);
    walkSprite.spriteClips.push_back(walk5);
    walkSprite.spriteClips.push_back(walk6);
    walkSprite.spriteClips.push_back(walk6);
    walkSprite.spriteClips.push_back(walk6);
    walkSprite.spriteClips.push_back(walk6);
    walkSprite.spriteClips.push_back(walk6);

    walkSprite.size.width  = DEFAULT_SPRITE.width * SPRITE_SCALE_FACTOR;
    walkSprite.size.height = DEFAULT_SPRITE.height * SPRITE_SCALE_FACTOR;

    auto &jumpSprite = reg.emplace<JumpSprite>(e);
    SDL_CHECK(loadSpriteFromFile(renderer, jumpSprite, "assets/player/Woodcutter_jump_red.png"));

    jumpSprite.total_frames = 30;    
    SDL_Rect jump1{ 0, 0, 40, 45};
    SDL_Rect jump2{ 45, 0, 40, 45};
    SDL_Rect jump3{ 90, 0, 40, 45};
    SDL_Rect jump4{ 135, 0, 40, 45};
    SDL_Rect jump5{ 180, 0, 40, 45};
    SDL_Rect jump6{ 225, 0, 40, 45};

    jumpSprite.spriteClips.push_back(jump1);
    jumpSprite.spriteClips.push_back(jump1);
    jumpSprite.spriteClips.push_back(jump1);
    jumpSprite.spriteClips.push_back(jump1);
    jumpSprite.spriteClips.push_back(jump1);
    jumpSprite.spriteClips.push_back(jump1);
    jumpSprite.spriteClips.push_back(jump1);
    jumpSprite.spriteClips.push_back(jump1);
    jumpSprite.spriteClips.push_back(jump1);
    jumpSprite.spriteClips.push_back(jump1);
    jumpSprite.spriteClips.push_back(jump2);
    jumpSprite.spriteClips.push_back(jump2);
    jumpSprite.spriteClips.push_back(jump2);
    jumpSprite.spriteClips.push_back(jump2);
    jumpSprite.spriteClips.push_back(jump2);
    jumpSprite.spriteClips.push_back(jump2);
    jumpSprite.spriteClips.push_back(jump2);
    jumpSprite.spriteClips.push_back(jump2);
    jumpSprite.spriteClips.push_back(jump2);
    jumpSprite.spriteClips.push_back(jump2);
    jumpSprite.spriteClips.push_back(jump3);
    jumpSprite.spriteClips.push_back(jump3);
    jumpSprite.spriteClips.push_back(jump3);
    jumpSprite.spriteClips.push_back(jump3);
    jumpSprite.spriteClips.push_back(jump3);
    jumpSprite.spriteClips.push_back(jump3);
    jumpSprite.spriteClips.push_back(jump3);
    jumpSprite.spriteClips.push_back(jump3);
    jumpSprite.spriteClips.push_back(jump3);
    jumpSprite.spriteClips.push_back(jump3);
    jumpSprite.spriteClips.push_back(jump4);
    jumpSprite.spriteClips.push_back(jump4);
    jumpSprite.spriteClips.push_back(jump4);
    jumpSprite.spriteClips.push_back(jump4);
    jumpSprite.spriteClips.push_back(jump4);
    jumpSprite.spriteClips.push_back(jump4);
    jumpSprite.spriteClips.push_back(jump4);
    jumpSprite.spriteClips.push_back(jump4);
    jumpSprite.spriteClips.push_back(jump4);
    jumpSprite.spriteClips.push_back(jump4);
    jumpSprite.spriteClips.push_back(jump5);
    jumpSprite.spriteClips.push_back(jump5);
    jumpSprite.spriteClips.push_back(jump5);
    jumpSprite.spriteClips.push_back(jump5);
    jumpSprite.spriteClips.push_back(jump5);
    jumpSprite.spriteClips.push_back(jump5);
    jumpSprite.spriteClips.push_back(jump5);
    jumpSprite.spriteClips.push_back(jump5);
    jumpSprite.spriteClips.push_back(jump5);
    jumpSprite.spriteClips.push_back(jump5);
    jumpSprite.spriteClips.push_back(jump6);
    jumpSprite.spriteClips.push_back(jump6);
    jumpSprite.spriteClips.push_back(jump6);
    jumpSprite.spriteClips.push_back(jump6);
    jumpSprite.spriteClips.push_back(jump6);
    jumpSprite.spriteClips.push_back(jump6);
    jumpSprite.spriteClips.push_back(jump6);
    jumpSprite.spriteClips.push_back(jump6);
    jumpSprite.spriteClips.push_back(jump6);
    jumpSprite.spriteClips.push_back(jump6);

    jumpSprite.size.width  = DEFAULT_SPRITE.width * SPRITE_SCALE_FACTOR;
    jumpSprite.size.height = DEFAULT_SPRITE.height * SPRITE_SCALE_FACTOR;

    // Collision boxes for the sprites.
    // IDLE COLLISION BOX
    Position cBoxPosition{18 * SPRITE_SCALE_FACTOR, 9 * SPRITE_SCALE_FACTOR};
    Size     cBoxSize{(int)(14 * SPRITE_SCALE_FACTOR), (int)(36 * SPRITE_SCALE_FACTOR)};

    makeCollisionBox(reg, e, cBoxPosition, cBoxSize, IDLE, LOOKING_RIGHT);

    cBoxPosition.x = idleSprite.size.width - (cBoxPosition.x + cBoxSize.width);
    makeCollisionBox(reg, e, cBoxPosition, cBoxSize, IDLE, LOOKING_LEFT);   

    // WALK COLLISION BOX
    cBoxPosition = {17 * SPRITE_SCALE_FACTOR, 8 * SPRITE_SCALE_FACTOR};
    cBoxSize = {(int)(13 * SPRITE_SCALE_FACTOR), (int)(37 * SPRITE_SCALE_FACTOR)};

    makeCollisionBox(reg, e, cBoxPosition, cBoxSize, WALK_RIGHT, LOOKING_RIGHT);

    cBoxPosition.x = walkSprite.size.width - (cBoxPosition.x + cBoxSize.width);
    makeCollisionBox(reg, e, cBoxPosition, cBoxSize, WALK_LEFT, LOOKING_LEFT);    

    // JUMP COLLISION BOX
    cBoxPosition = {13 * SPRITE_SCALE_FACTOR, 12 * SPRITE_SCALE_FACTOR};
    cBoxSize = {(int)(15 * SPRITE_SCALE_FACTOR), (int)(35 * SPRITE_SCALE_FACTOR)};

    makeCollisionBox(reg, e, cBoxPosition, cBoxSize, JUMP, LOOKING_RIGHT);

    cBoxPosition.x = walkSprite.size.width - (cBoxPosition.x + cBoxSize.width);
    makeCollisionBox(reg, e, cBoxPosition, cBoxSize, JUMP, LOOKING_LEFT);    

    // RUN COLLISION BOX
    cBoxPosition = {15 * SPRITE_SCALE_FACTOR, 11 * SPRITE_SCALE_FACTOR};
    cBoxSize = {(int)(14 * SPRITE_SCALE_FACTOR), (int)(34 * SPRITE_SCALE_FACTOR)};

    makeCollisionBox(reg, e, cBoxPosition, cBoxSize, RUN_RIGHT, LOOKING_RIGHT);

    cBoxPosition.x = walkSprite.size.width - (cBoxPosition.x + cBoxSize.width);
    makeCollisionBox(reg, e, cBoxPosition, cBoxSize, RUN_LEFT, LOOKING_LEFT);    
    return e;
}

entt::entity makeLevel(entt::registry &reg, GameSettings game, float widthPer, float heightPer, Color color, std::string name){
    const entt::entity e = reg.create();
    auto &level          = reg.emplace<Level>(e);
    auto &size           = reg.emplace<Size>(e);
    auto &position       = reg.emplace<Position>(e);
    auto &colorComp      = reg.emplace<Color>(e);

    level.name = name;

    size.width  = game.WIDTH  * widthPer;
    size.height = game.HEIGHT * heightPer;

    position.x = (game.WIDTH / 2) - (size.width / 2);
    position.y = (game.HEIGHT / 2) - (size.height / 2);

    colorComp   = color;

    level.floor = position.y + size.height;
    
    return e;
}


entt::entity makePlatform(entt::registry &reg, GameSettings game, entt::entity levelEntity, Position position, Size size, Color color){
    const entt::entity e   = reg.create();
    auto &platform         = reg.emplace<Platform>(e);
    auto &platformSize     = reg.emplace<Size>(e);
    auto &platformPosition = reg.emplace<Position>(e);
    auto &platformColor    = reg.emplace<Color>(e);

    platform.level = levelEntity;
    platformSize  = size;
    platformPosition = position;
    platformColor = color;
    return e;
}