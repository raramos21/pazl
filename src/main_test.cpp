#include <iostream>
#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <string>
#include <ctime>
#include <vector>
#include <math.h>

#include "Math/Mat44.hpp"
#include <entt/entt.hpp>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 740;

const int PLAYER_IDLE_FRAMES  = 4;
const int PLAYER_ATTACK_FRAMES = 6;

bool quit = false;
int frame = 0;

class LTexture{
    public:
        LTexture();
        ~LTexture();
        
        bool loadFromFile(std::string path);
        void free();
        void setColor(Uint8 red, Uint8 green, Uint8 blue);
        void setBlendMode(SDL_BlendMode blending);
        void setAlpha(Uint8 alpha);
        void render(int x, int y, bool scaled, SDL_Rect* clip = NULL);
        
        int getWidth();
        int getHeight();

    private:
        SDL_Texture* mTexture;

        int mWidth;
        int mHeight;
};

SDL_Window*   gWindow        = NULL;
SDL_Renderer* gRenderer      = NULL;

LTexture      gPlayerIdleTexture;
LTexture      gPlayerAttackTexture;
SDL_Rect      gPlayerIdleSpriteClips[PLAYER_IDLE_FRAMES];
SDL_Rect      gPlayerAttackSpriteclips[PLAYER_ATTACK_FRAMES];
TTF_Font*     gFont = NULL;

SDL_Event e;

LTexture::LTexture(){
    mTexture = NULL;
    mWidth   = 0;
    mHeight  = 0;
}

LTexture::~LTexture(){
    free();
}

bool LTexture::loadFromFile(std::string path){
    free();

    SDL_Texture* newTexture    = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    
    if(loadedSurface == NULL){
        printf("Unable to load image %s! SDL_image error: %s\n", path.c_str(), IMG_GetError());
    } else {
        
        // SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0x00, 0xFF, 0xFF));
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

        if(newTexture == NULL){
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        } else {    
            mWidth  = loadedSurface->w;
            mHeight = loadedSurface->h;
        }
        SDL_FreeSurface(loadedSurface);
    }

    mTexture = newTexture;
    return mTexture != NULL;
}

void LTexture::free() {
    if(mTexture != NULL){
        mTexture = NULL;
        mWidth   = 0;
        mHeight  = 0;
    }
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue){
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending){
    SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha){
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, bool scaled, SDL_Rect* clip){
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};
    
    if(scaled){
        renderQuad = {x, y, SCREEN_WIDTH, SCREEN_HEIGHT};    
    }

    if(clip != NULL){
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    
    SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

int LTexture::getWidth(){
    return mWidth;
}

int LTexture::getHeight(){
    return mHeight;
}

bool init();

bool init() {
    bool success = true;

    if( SDL_Init(SDL_INIT_VIDEO) < 0) {        
        printf("SDL could not initialize! SDL_ERROR: %s \n", SDL_GetError());
        success = false;
    } else {
        gWindow = SDL_CreateWindow("SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL_ERROR: %s\n", SDL_GetError());
            success = false;
        } else {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);// | SDL_RENDERER_PRESENTVSYNC);
            // gScreenSurface = SDL_GetWindowSurface(gWindow);
            if(gRenderer == NULL){
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            } else {
                // SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0xFF, 0xFF);

                int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
                if(!(IMG_Init(imgFlags) & imgFlags)){
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }

                if( TTF_Init() == -1 )
                {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    success = false;
                }
            }
        }
    }

    return success;
}

//ENTITY
struct Entity {
    Uint64 id;
};

//COMPONENTS
struct Position {    
    float x;
    float y;
};

struct Player : public Entity {
    bool user_controlled = true;

};


struct State{
    float x; // x-position
    // float y; // y-position
    float v; // velocity
};

struct Derivative{
    float dx; // dx/dt = velocity
    float dv; // dv/dt = acceleration
};

Derivative evaluate(const State &initial, double t, float dt, const Derivative &d){
    State state;
    state.x = initial.x + d.dx*dt;
    state.v = initial.v + d.dv*dt;
    
    Derivative output;
    output.dx = 0.0;
    output.dv = 0.0;
    return output;
}

float acceleration(const State &state, double t){
    const float k = 15.0f;
    const float b = 0.1f;

    return -k * state.x - b * state.v;
}

void integrate(State &state, double t, float dt){
    Derivative a, b, c, d;

    a = evaluate(state, t, 0.0f, Derivative());
    b = evaluate(state, t, dt*0.5f, a);
    c = evaluate(state, t, dt*0.5f, b);
    d = evaluate(state, t, dt, c);

    float dxdt = 1.0f / 6.0f * (a.dx + 2.0f  * (b.dx + c.dx) + d.dx);
    float dvdt = 1.0f / 6.0f * (a.dv + 2.0f  * (b.dv + c.dv) + d.dv);

    state.x = state.x + dxdt * dt;
    state.v = state.v + dvdt * dt;
}

void semiImplicitEuler(){
    double t = 0.0;
    float dt = 1.0/100;

    float velocity = 0.0f;
    float position = 0.0f;
    float force    = 10.0f;
    float mass     = 1.0f;

    while( t <= 10.0){
        // printf("t=%f, position=%f, velocity=%f, dt=%f\n", t, position, velocity, dt);
        printf("%f, %f, %f, %f\n", t, position, velocity, dt);
        velocity += (force/mass) * dt;
        position += velocity * dt;
        
        t += dt;        
    }
}

void render(){
    SDL_Rect viewport = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderSetViewport(gRenderer, &viewport);            
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    SDL_Rect* currentClip = &gPlayerIdleSpriteClips[frame/4];
    gPlayerIdleTexture.render((SCREEN_WIDTH- currentClip->w)/2, (SCREEN_HEIGHT - currentClip->h)/2, false, currentClip);
    
    SDL_RenderPresent(gRenderer);

    frame += 1;

    if(frame / 4 >= PLAYER_IDLE_FRAMES){
        frame = 0;
    }
    // SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
    // SDL_RenderPresent(gRenderer);
}

bool loadMedia() {
    bool success = true;
    char const* playerIdleTexture = "assets/1 Woodcutter/Woodcutter_idle.png";
    char const* playerAttackTexture = "assets/1 Woodcutter/Woodcutter_attack1.png";


    if(!gPlayerIdleTexture.loadFromFile(playerIdleTexture)){
        success = false;
    } else {
        //32*28
        gPlayerIdleSpriteClips[0].x = 0;
        gPlayerIdleSpriteClips[0].y = 16;
        gPlayerIdleSpriteClips[0].w = 32;
        gPlayerIdleSpriteClips[0].h = 28;

        gPlayerIdleSpriteClips[1].x = 48;
        gPlayerIdleSpriteClips[1].y = 16;
        gPlayerIdleSpriteClips[1].w = 32;
        gPlayerIdleSpriteClips[1].h = 28;

        gPlayerIdleSpriteClips[2].x = 95;
        gPlayerIdleSpriteClips[2].y = 16;
        gPlayerIdleSpriteClips[2].w = 32;
        gPlayerIdleSpriteClips[2].h = 28;

        gPlayerIdleSpriteClips[3].x = 143;
        gPlayerIdleSpriteClips[3].y = 16;
        gPlayerIdleSpriteClips[3].w = 32;
        gPlayerIdleSpriteClips[3].h = 28;
    }

    if(!gPlayerAttackTexture.loadFromFile(playerAttackTexture)){
        success = false;
    } else {
        //32*28
        gPlayerAttackSpriteclips[0].x = 0;
        gPlayerAttackSpriteclips[0].y = 16;
        gPlayerAttackSpriteclips[0].w = 32;
        gPlayerAttackSpriteclips[0].h = 28;

        gPlayerAttackSpriteclips[1].x = 48;
        gPlayerAttackSpriteclips[1].y = 16;
        gPlayerAttackSpriteclips[1].w = 32;
        gPlayerAttackSpriteclips[1].h = 28;

        gPlayerAttackSpriteclips[2].x = 95;
        gPlayerAttackSpriteclips[2].y = 16;
        gPlayerAttackSpriteclips[2].w = 32;
        gPlayerAttackSpriteclips[2].h = 28;

        gPlayerAttackSpriteclips[3].x = 143;
        gPlayerAttackSpriteclips[3].y = 16;
        gPlayerAttackSpriteclips[3].w = 32;
        gPlayerAttackSpriteclips[3].h = 28;
    }

    const char *font = "assets/Roboto-Light.ttf";
    gFont = TTF_OpenFont(font, 24);
	if ( gFont == NULL ) {
		printf("Error loading font: %s\n", TTF_GetError());
		success = false;
	}

    return success;
}


void renderText(std::string text, SDL_Rect dest) {
	SDL_Color fg = { 0, 0xcd, 0x10 };
	SDL_Surface* surf = TTF_RenderText_Solid(gFont, text.c_str(), fg);

	dest.w = surf->w;
	dest.h = surf->h;

	SDL_Texture* tex = SDL_CreateTextureFromSurface(gRenderer, surf);

	SDL_RenderCopy(gRenderer, tex, NULL, &dest);
	SDL_DestroyTexture(tex);
	SDL_FreeSurface(surf);
}

int main(int argc, char* args []){
    if(!init()){
        printf("Failed to initialize!\n");
        return 0;
    }

    if(!loadMedia()){
        printf("Failed to load media!\n");
        return 0;
    }

    entt::registry registry;

    double t = 0.0;
    float dt = 0.1;

    double currentTime = SDL_GetTicks() / 1000.0; //SECONDS
    double accumulator = 0.0;
    
    State previousState;
    State currentState;

    Uint32 totalFrameTicks = 0;
    Uint32 totalFrames = 0;
   
    while(!quit){
        double newTime    = SDL_GetTicks() / 1000.0;
        double frameTime  = newTime - currentTime;

        Uint32 startTicks = SDL_GetTicks();
        Uint64 startPerf  = SDL_GetPerformanceCounter();
        totalFrames++;

        if(frameTime > 0.25)
            frameTime = 0.25;
        
        currentTime = newTime;
        accumulator += frameTime;

        // Proccess user input!
        while(SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT){
                quit = true;
            } else if(e.type == SDL_KEYDOWN){
                switch(e.key.keysym.sym){
                    case SDLK_d:
                        printf("MOVE RIGHT!\n");                        
                        break;
                    case SDLK_a:
                        printf("MOVE LEFT!\n");
                        break;
                    case SDLK_SPACE:
                        printf("JUMP!\n");
                        break;
                    case SDLK_LSHIFT:
                        printf("ACCELERATE/RUN!\n");
                        break;
                    default:
                        break;
                }
            }
        }

        while(accumulator >= dt){
            previousState = currentState;
            //integrate(currentState, t, dt)
            t += dt;
            accumulator -= dt;
        }

        const double alpha = accumulator / dt;

        State state;
        // state = currentState * alpha + previousState * (1.0 - alpha);

        render();
        

        Uint32 endTicks  = SDL_GetTicks();
        Uint64 endPerf   = SDL_GetPerformanceCounter();
        Uint64 framePerf = endPerf - startPerf;
        float frameTimeSDL  = (endTicks - startTicks) / 1000.0f;
        totalFrameTicks  += endTicks - startTicks;

        std::string fps = "Current FPS: " + std::to_string(1.0f / frameTimeSDL);
		std::string avg = "Average FPS: " + std::to_string(1000.0f / ((float)totalFrameTicks / totalFrames));
		std::string perf = "Current Perf: " + std::to_string(framePerf);

        // SDL_RenderClear(gRenderer);
        SDL_Rect dest = { 10, 10, 0, 0 };
		renderText(fps, dest);
		dest.y += 24;
		renderText(avg, dest);
		dest.y += 24;
		renderText(perf, dest);
        SDL_RenderPresent(gRenderer);

        if( (frameTimeSDL * 1000) < (1000/ 120) ) 
        {            
            //Sleep the remaining frame time
            SDL_Delay( (1000/ 120) - (frameTimeSDL * 1000) );
        } 
    }
    

    return 0;
}