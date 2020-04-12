#include "GameEntity.h"

GameEntity* GameEntity::flame = new GameEntity;
GameEntity* GameEntity::bullet = new GameEntity;

GameEntity::GameEntity()
{
    this->pre = NULL;
    this->next = NULL;
    this->animation = NULL;
    this->physic = NULL;
}

GameEntity::GameEntity(SPRITE_SHEET_NAME texture_id, const int& init_pos_x, const int& init_pos_y, int& properties)
{
    this->id = texture_id;
    this->physic = new GamePhysics(init_pos_x, init_pos_y);

    SDL_QueryTexture(GameTexture::Texture[id], NULL, NULL, &(this->width), NULL);
    this->animation = new GameAnimation(this->width, this->width, this->width, this->width);
    this->properties = properties;

    this->angle = angle;

    if(this->id == FLAME_BULLET)
    {
        this->next = flame->GetNext();
        this->pre = flame;
        if(!(flame->GetNext() == NULL))
        {
            flame->GetNext()->SetPre(this);
        }
        flame->SetNext(this);
    }
    else
    {
        this->next = bullet->GetNext();
        this->pre = bullet;
        if(!(bullet->GetNext() == NULL))
        {
            bullet->GetNext()->SetPre(this);
        }
        bullet->SetNext(this);
    }

    ExpireTime = -1;
}

GameEntity::GameEntity(SPRITE_SHEET_NAME texture_id, const int& init_pos_x, const int& init_pos_y, const double& angle, int& properties)
{
    this->id = texture_id;
    this->physic = new GamePhysics(init_pos_x, init_pos_y);

    SDL_QueryTexture(GameTexture::Texture[id], NULL, NULL, &(this->width), NULL);
    this->animation = new GameAnimation(this->width, this->width, this->width, this->width);
    this->properties = properties;

    this->angle = angle;

    if(this->id == FLAME_BULLET)
    {
        this->next = flame->GetNext();
        this->pre = flame;
        if(!(flame->GetNext() == NULL))
        {
            flame->GetNext()->SetPre(this);
        }
        flame->SetNext(this);
    }
    else
    {
        this->next = bullet->GetNext();
        this->pre = bullet;
        if(!(bullet->GetNext() == NULL))
        {
            bullet->GetNext()->SetPre(this);
        }
        bullet->SetNext(this);
    }

    if(this->id == FLAME_BULLET)
    {
        this->width = 21;
        this->angle = 0.0;

        this->physic->SetVx(400 * cos(angle));
        this->physic->SetVy(400 * sin(angle));

        this->physic->SetUpperVx(max(0.0, 800 * cos(angle)));
        this->physic->SetLowerVx(min(0.0, 800 * cos(angle)));
        this->physic->SetUpperVy(max(0.0, 800 * sin(angle)));
        this->physic->SetLowerVy(min(0.0, 800 * sin(angle)));

        ExpireTime = SDL_GetTicks() + 1000;
    }
    else if(this->id == GUN_BULLET)
    {
        this->physic->SetVx(600 * cos(angle));
        this->physic->SetVy(600 * sin(angle));

        this->physic->SetUpperVx(max(0.0, 800 * cos(angle)));
        this->physic->SetLowerVx(min(0.0, 800 * cos(angle)));
        this->physic->SetUpperVy(max(0.0, 800 * sin(angle)));
        this->physic->SetLowerVy(min(0.0, 800 * sin(angle)));

        ExpireTime = SDL_GetTicks() + 2500;
    }
}

GameEntity::~GameEntity()
{
    delete animation;
    delete physic;
}

void GameEntity::Update(const Uint32& GameTime)
{
    if(this == NULL) return;
    if(!(this->next == NULL))
    {
        this->next->Update(GameTime);
    }
    if(this->ExpireTime < GameTime)
    {
        this->pre->SetNext(this->next);
        if(!(this->next == NULL))
        {
            this->next->SetPre(this->pre);
        }
        delete this;
        return;
    }
    this->animation->Update(GameTime);
    this->physic->Update(GameTime);
    int pos_x;
    int pos_y;
    this->physic->GetPos(pos_x, pos_y);

    if(this->properties & PROGRESSIVE_GROW)
    {
        if(this->width <= 78)
        {
            this->width += 1;
        }
    }
}

void GameEntity::Render(SDL_Renderer* Ren, const int& screen_pos_x, const int& screen_pos_y)
{
    if(this == NULL) return;
    if(!(this->next == NULL))
    {
        this->next->Render(Ren, screen_pos_x, screen_pos_y);
    }
    int pos_x;
    int pos_y;
    this->physic->GetPos(pos_x, pos_y);

    SDL_Rect e = {pos_x - int(this->width / 2) - screen_pos_x, pos_y - int(this->width / 2) - screen_pos_y, this->width, this->width};

    if(e.x > SCREEN_WIDTH || e.x + e.w < 0 || e.y > SCREEN_HEIGHT || e.y + e.h < 0)
    {
        return;
    }

    SDL_RenderCopyEx(Ren, GameTexture::Texture[id], NULL, &e, angle, NULL, SDL_FLIP_NONE);
}

bool GameEntity::Collision(const SDL_Rect& HitBox, const int& properties)
{
    bool coll = false;

    coll = this->physic->Collision(HitBox, this->id, this->width/2);

    if(coll == true)
    {
        bool deal_dame = false;

        if((this->properties & DEAL_DAME) && (properties & GET_DAME))
        {
            deal_dame = true;
        }

        if(this->id == GUN_BULLET)
        {
            this->pre->SetNext(this->next);
            if(!(this->next == NULL))
            {
                this->next->SetPre(this->pre);
            }
            delete this;
        }

        return deal_dame;
    }

    return false;
}

bool GameEntity::Collision(const SDL_Point& Center, const int& r, const int& properties)
{
    bool coll = false;

    coll = this->physic->Collision(Center, this->id, this->width / 2, r);

    if(coll == true)
    {
        bool deal_dame = false;

        if((this->properties & DEAL_DAME) && (properties & GET_DAME))
        {
            deal_dame = true;
        }

        if(this->id == GUN_BULLET)
        {
            this->pre->SetNext(this->next);
            if(!(this->next == NULL))
            {
                this->next->SetPre(this->pre);
            }
            delete this;
        }

        return deal_dame;
    }

    return false;
}

GameEntity* GameEntity::GetPre()
{
    return this->pre;
}

GameEntity* GameEntity::GetNext()
{
    return this->next;
}

void GameEntity::SetPre(GameEntity* e)
{
    this->pre = e;
}

void GameEntity::SetNext(GameEntity* e)
{
    this->next = e;
}

void GameEntity::GetStatus(int &id, int &properties)
{
    id = this->id;
    properties = this->properties;
}

