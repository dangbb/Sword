#ifndef _GAME_ENTITY_H_
#define _GAME_ENTITY_H_

#include "GameOptions.h"
#include "GameAnimate.h"
#include "GameTexture.h"
#include "GamePhysics.h"

class GameEntity
{
    /// use for bullet rendering
    public:
        static GameEntity* flame;
        static GameEntity* bullet;

        GameEntity();
        GameEntity(SPRITE_SHEET_NAME texture_id, const int& init_pos_x, const int& init_pos_y, int& properties);
        GameEntity(SPRITE_SHEET_NAME texture_id, const int& init_pos_x, const int& init_pos_y, const double& angle, int& properties);
        ~GameEntity();

        void Render(SDL_Renderer* Ren, const int& screen_pos_x, const int& screen_pos_y);
        void Update(const Uint32& GameTime);
        bool Collision(const SDL_Point& Center, const int& r, const int& properties);
        bool Collision(const SDL_Rect& HitBox, const int& properties);
        void GetStatus(int& id, int& properties);

        GameEntity* GetPre();
        void SetPre(GameEntity* e);
        GameEntity* GetNext();
        void SetNext(GameEntity* e);

    private:
        GameEntity* next;
        GameEntity* pre;

        SPRITE_SHEET_NAME id;

        int speed;
        int width;
        int properties;

        double angle;

        Uint32 ExpireTime;

        GameAnimation* animation;
        GamePhysics* physic;
};

#endif // _GAME_ENTITY_H_
