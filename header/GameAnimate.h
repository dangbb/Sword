#ifndef _GAME_ANIMATE_H_
#define _GAME_ANIMATE_H_

#include "GameOptions.h"

class GameAnimation
{
    public:
        GameAnimation(const int& width, const int& height, const int& frame_width, const int& frame_height);
        ~GameAnimation();

        void SetMode(int state_y, Uint32 Timer, int TimeGap, int state_x, ANIMATION_MODE e);
        void SetMode();
        void GetState(int& state_y);
        void Update(const Uint32& GameTime);
        void Source(SDL_Rect &r);
        void GetTimer(Uint32& Timer) const;

    protected:
        bool running;

        int width;
        int height;
        int frame_width;
        int frame_height;

        int state_x;
        int state_y;
        ANIMATION_MODE e;

        Uint32 Timer;
        int TimeGap;
};

#endif // _GAME_ANIMATE_H_
