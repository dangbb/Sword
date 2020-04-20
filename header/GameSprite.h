#ifndef _GAME_SPRITE_H_
#define _GAME_SPRITE_H_

#include "GameAnimate.h"
#include "GameOptions.h"
#include "GameTexture.h"

class GameSprite
{
    public:
        static bool SortPolicy(const GameSprite* SpriteA, const GameSprite* SpriteB);
        GameSprite(int texture_id, const int& init_pos_x, const int& init_pos_y);
        ~GameSprite();

        void Render(SDL_Renderer* Ren, const int& screen_pos_x, const int& screen_pos_y);
        void GetProperties(int& properties);
        void GetHitbox(SDL_Rect& Dest) const;

    protected:
        int id;
        int properties;

        SDL_Rect Dest;
};

#endif // _GAME_SPRITE_H_
