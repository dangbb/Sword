#include "GameSprite.h"

bool GameSprite::SortPolicy(const GameSprite* SpriteA, const GameSprite* SpriteB)
{
    SDL_Rect a;
    SDL_Rect b;

    SpriteA->GetHitbox(a);
    SpriteB->GetHitbox(b);

    return a.y < b.y;
}

GameSprite::GameSprite(int texture_id, const int& init_pos_x, const int& init_pos_y)
{
    this->id = texture_id;

    int width;
    int height;

    SDL_QueryTexture(GameTexture::Texture[id], NULL, NULL, &width, &height);

    Dest.x = init_pos_x - width / 2;
    Dest.y = init_pos_y - height / 2;
    Dest.w = width;
    Dest.h = height;

    properties = BLOCK_BULLET;
}

GameSprite::~GameSprite()
{
}

void GameSprite::GetProperties(int& properties)
{
    properties = this->properties;
}

void GameSprite::Render(SDL_Renderer* Ren, const int& screen_pos_x, const int& screen_pos_y)
{
    Dest.x -= screen_pos_x;
    Dest.y -= screen_pos_y;

    if(Dest.x > SCREEN_WIDTH || Dest.x + Dest.w < 0 || Dest.y > SCREEN_HEIGHT || Dest.y + Dest.h < 0)
    {

    }
    else
    {
        SDL_RenderCopy(Ren, GameTexture::Texture[id], NULL, &Dest);
    }

    Dest.x += screen_pos_x;
    Dest.y += screen_pos_y;
}

void GameSprite::GetHitbox(SDL_Rect& Dest) const
{
    Dest = this->Dest;
}
