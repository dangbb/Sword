#include "GamePhysics.h"

GamePhysics::GamePhysics(int init_pos_x, int init_pos_y)
{
    pos_x = init_pos_x;
    pos_y = init_pos_y;
    v_x = 0;
    v_y = 0;
    a_x = 0;
    a_y = 0;
    upper_v_x = 0;
    upper_v_y = 0;
    lower_v_x = 0;
    lower_v_y = 0;

    Timer = 0;
}

GamePhysics::~GamePhysics(){}

void GamePhysics::Update(Uint32 GameTime)
{
    int SPF = 1000 / FPS;

    if(Timer == 0)
    {
        Timer = GameTime;
    }
    else
    {
        int milisec_per_frame = 1000 / FPS;

        if(GameTime - Timer >= milisec_per_frame)
        {
            v_x = v_x + a_x * (1.0f * SPF / 1000);
            v_y = v_y + a_y * (1.0f * SPF / 1000);

            if(v_x > upper_v_x) v_x = upper_v_x;
            if(v_y > upper_v_y) v_y = upper_v_y;

            if(v_x < lower_v_x) v_x = lower_v_x;
            if(v_y < lower_v_y) v_y = lower_v_y;

            pos_x = pos_x + v_x * (1.0f * SPF / 1000);
            pos_y = pos_y + v_y * (1.0f * SPF / 1000);

            Timer = GameTime;
        }
    }
}

void GamePhysics::SetVx(int v_x)
{
    this->v_x = v_x;
}

void GamePhysics::SetVy(int v_y)
{
    this->v_y = v_y;
}

void GamePhysics::SetAx(int a_x)
{
    this->a_x = a_x;
}

void GamePhysics::SetAy(int a_y)
{
    this->a_y = a_y;
}

void GamePhysics::SetUpperVx(int upper_v_x)
{
    this->upper_v_x = upper_v_x;
}

void GamePhysics::SetUpperVy(int upper_v_y)
{
    this->upper_v_y = upper_v_y;
}

void GamePhysics::SetLowerVx(int lower_v_x)
{
    this->lower_v_x = lower_v_x;
}

void GamePhysics::SetLowerVy(int lower_v_y)
{
    this->lower_v_y = lower_v_y;
}

void GamePhysics::GetPos(int& pos_x, int& pos_y) const
{
    pos_x = int(this->pos_x);
    pos_y = int(this->pos_y);
}

bool GamePhysics::Collision(const SDL_Point& Center, const int& id, const int& ra, const int& rb)
{
    int pos_x = int(this->pos_x);
    int pos_y = int(this->pos_y);

    bool coll = false;

    if(TextureManagement::Distance(Center.x, pos_x, Center.y, pos_y) <= ra + rb)
    {
        coll = true;
    }
    return coll;
}

bool GamePhysics::Collision(const SDL_Rect& HitBox, const int& id, const int& r)
{
    int pos_x = int(this->pos_x);
    int pos_y = int(this->pos_y);

    bool coll = false;

    if(TextureManagement::Collision(SDL_Rect{HitBox.x - r, HitBox.y, HitBox.w + 2 * r, HitBox.h}, SDL_Rect{pos_x, pos_y, 0, 0}))
    {
        if(id == PLAYER || id == FLAME_BOT || id == GUN_BOT)
        {
            if(this->pos_x < HitBox.x + HitBox.w / 2)
            {
                this->pos_x = HitBox.x - r;
                /// move one pixel to the right to avoid collision
            }
            else
            {
                this->pos_x = HitBox.x + HitBox.w + r;
                /// one pixel to the left
            }
        }
        coll = true;
    }
    else if(TextureManagement::Collision(SDL_Rect{HitBox.x, HitBox.y - r, HitBox.w, HitBox.h + 2 * r}, SDL_Rect{pos_x, pos_y, 0, 0}))
    {
        if(id == PLAYER || id == FLAME_BOT || id == GUN_BOT)
        {
            if(this->pos_y < HitBox.y + HitBox.h / 2)
            {
                this->pos_y = HitBox.y - r;
            }
            else
            {
                this->pos_y = HitBox.y + HitBox.h + r;
            }
        }
        coll = true;
    }
    else if(TextureManagement::Distance(pos_x, HitBox.x, pos_y, HitBox.y) <= r)
    {
        if(id == PLAYER || id == FLAME_BOT || id == GUN_BOT)
        {
            double angle = atan2(pos_y - HitBox.y + ESL, pos_x - HitBox.x);
            this->pos_x -= 1 * cos(angle);
            this->pos_y -= 1 * sin(angle);
            /// increase if not enough
        }
        coll = true;
    }
    else if(TextureManagement::Distance(pos_x, HitBox.x + HitBox.w, pos_y, HitBox.y) <= r)
    {
        if(id == PLAYER || id == FLAME_BOT || id == GUN_BOT)
        {
            double angle = atan2(pos_y - HitBox.y + ESL, pos_x - HitBox.x);
            this->pos_x -= 1 * cos(angle);
            this->pos_y -= 1 * sin(angle);
        }
        coll = true;
    }
    else if(TextureManagement::Distance(pos_x, HitBox.x, pos_y, HitBox.y + HitBox.h) <= r)
    {
        if(id == PLAYER || id == FLAME_BOT || id == GUN_BOT)
        {
            double angle = atan2(pos_y - HitBox.y + ESL, pos_x - HitBox.x);
            this->pos_x -= 1 * cos(angle);
            this->pos_y -= 1 * sin(angle);
        }
        coll = true;
    }
    else if(TextureManagement::Distance(pos_x, HitBox.x + HitBox.w, pos_y, HitBox.y + HitBox.h) <= r)
    {
        if(id == PLAYER || id == FLAME_BOT || id == GUN_BOT)
        {
            double angle = atan2(pos_y - HitBox.y + ESL, pos_x - HitBox.x);
            this->pos_x -= 1 * cos(angle);
            this->pos_y -= 1 * sin(angle);
        }
        coll = true;
    }

    if(coll && id == FLAME_BULLET)
    {
        a_x = 0;
        v_x = 0;
        a_y = 0;
        v_y = 0;
    }

    return coll;
}
