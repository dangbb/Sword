#ifndef _GAME_PHYSICS_H_
#define _GAME_PHYSICS_H_

#include "GameOptions.h"
#include "GameSprite.h"

class GamePhysics
{
    public:
        GamePhysics(int init_pos_x, int init_pos_y);
        ~GamePhysics();

        void Update(Uint32 GameTime);
        void SetVx(int v_x);
        void SetVy(int v_y);
        void SetAx(int a_x);
        void SetAy(int a_y);

        void SetUpperVx(int upper_v_x);
        void SetUpperVy(int upper_v_y);
        void SetLowerVx(int lower_v_x);
        void SetLowerVy(int lower_v_y);

        void GetPos(int &pos_x, int &pos_y) const;

        bool Collision(const SDL_Point& Center, const int& id, const int& ra, const int& rb);
        bool Collision(const SDL_Rect& HitBox, const int& id, const int& r);

    protected:
        float pos_x;
        float pos_y;
        float v_x;
        float v_y;
        float a_x;
        float a_y;
        float upper_v_x;
        float upper_v_y;
        float lower_v_x;
        float lower_v_y;

        Uint32 Timer;
};

#endif // _GAME_PHYSICS_H_
