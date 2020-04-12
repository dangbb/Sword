#include "GameAnimate.h"

GameAnimation::GameAnimation(const int& width, const int& height, const int& frame_width, const int& frame_height)
{
    this->running = false;
    this->width = width;
    this->height = height;

    this->frame_width = frame_width;
    this->frame_height = frame_height;

    this->state_x = 0;
    this->state_y = 0;

    e = CYCLE;
    Timer = 0;
    TimeGap = 200;
}

GameAnimation::~GameAnimation()
{

}

void GameAnimation::GetState(int& state_x)
{
    state_x = this->state_x;
}

void GameAnimation::SetMode(int state_y, Uint32 Timer, int TimeGap, int state_x, ANIMATION_MODE e)
{
    this->running = true;
    this->e = e;
    this->state_y = state_y;
    this->state_x = state_x;
    this->Timer = Timer;
    this->TimeGap = TimeGap;
}

void GameAnimation::Update(const Uint32& GameTime)
{
    if(!running) return;
    if(GameTime - Timer > TimeGap)
    {
        Timer = GameTime;

        state_x ++;

        if(e == ONEWAY)
        {
            if(state_x == width / frame_width)
            {
                running = false;
            }
        }
        else
        {
            if(state_x == width / frame_width)
            {
                state_x = 0;
            }
        }
    }
}

void GameAnimation::Source(SDL_Rect& r)
{
    r.x = state_x * frame_width;
    r.y = state_y * frame_height;
    r.w = frame_width;
    r.h = frame_height;
}

void GameAnimation::GetTimer(Uint32& Timer) const
{
    Timer = this->Timer;
}

void GameAnimation::SetMode()
{
    state_x = 1;
    state_y = 0;
    Timer = 0;
    TimeGap = 0;
    e = ONEWAY;
}
