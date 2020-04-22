#ifndef _GAME_OPTION_H_
#define _GAME_OPTION_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <bits/stdc++.h>

#define GAME_PATH "anno/game_001.txt"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define FPS 60
#define ESL 1e-6
#define bullet_speed 1000
#define game_name "SwordmanX"

using namespace std;

typedef enum
{
    SE_WIN = 0,
    SE_FAIL,
    SE_CUT,
    SE_GUNSHOT,
    SE_FLAMESHOT,
    TOTAL
} SOUND_EFFECT;

typedef enum
{
    NEW_GAME,
    SANDBOX,
    EXIT
} GUI_EVENT;

typedef enum
{
    FLAME_BOT = 0,
    GUN_BOT,
    PLAYER,

    FLAME_BULLET,
    GUN_BULLET,
    CUT_SPRITE,

    BARREL,
    WALL,

    BURN_SYM,
    HEALTH_SYM,

    NUM
} SPRITE_SHEET_NAME;

typedef enum
{
    SLOWER = 0,
    SAME
} PHYSIC_MODE;

typedef enum
{
    ONEWAY = 0,
    CYCLE
} ANIMATION_MODE;

typedef enum {
    BLOCK_BULLET = 1 << 0,
    GET_DAME = 1 << 1,
    DEAL_DAME = 1 << 2,
    PROGRESSIVE_GROW = 1 << 3,
    FROM_ENEMY = 1 << 4,
    FROM_PLAYER = 1 << 5,
    BURN = 1 << 6
} SPRITE_EFFECT;

typedef enum {
    SAND = 0,
    GRASS,
    WATER,
    LAVA
} TERRAIN;

typedef enum {
    CONTROL = 0,
    DEAD
} PLAYER_STATUS;

typedef enum {
    USABLE,
    INUSE,
    DISABLE
} DIRECTION_STATE;

class TextureManagement
{
    public:
        static SDL_Texture* LoadTexture(SDL_Renderer* Ren, string path);
        static bool Collision(SDL_Rect a, SDL_Rect b);
        static double Distance(const double& xa, const double& xb, const double& ya, const double& yb);
        static bool IsLineCut(const SDL_Point& a, const SDL_Point& b, const SDL_Point& x, const SDL_Point& y);
        static bool ClockWise(const SDL_Point& a, const SDL_Point& b, const SDL_Point& x);
        static bool CounterClockWise(const SDL_Point& a, const SDL_Point& b, const SDL_Point& x);
};

const double PI = acos(-1);
const double Angle_Instance[6] = {PI, PI * 2 / 3, PI / 3, 0, -PI / 3, -2 * PI / 3};

#endif // _GAME_OPTION_H_
