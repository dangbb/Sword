#include "GameOptions.h"

SDL_Texture* TextureManagement::LoadTexture(SDL_Renderer* Ren, string path)
{
    SDL_Surface* LoadedSurface = NULL;
    SDL_Texture* ReturnTexture = NULL;

    if((LoadedSurface = IMG_Load(path.c_str())) == NULL)
    {
        printf("Unable to load image from %s : %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        SDL_SetColorKey(LoadedSurface, SDL_TRUE, SDL_MapRGB(LoadedSurface->format, 0xFF, 0xFF, 0xFF));

        ReturnTexture = SDL_CreateTextureFromSurface(Ren, LoadedSurface);

        if(ReturnTexture == NULL)
        {
            printf("Unable to create Texture %s\n", SDL_GetError());
        }
    }
    return ReturnTexture;
}

bool TextureManagement::Collision(SDL_Rect a, SDL_Rect b)
{
    int x = max(a.x, b.x) - min(a.x + a.w, b.x + b.w);
    int y = max(a.y, b.y) - min(a.y + a.h, b.y + b.h);

    if(x > 0 || y > 0) return false;

    return true;
}

double TextureManagement::Distance(const double& xa, const double& xb, const double& ya, const double& yb)
{
    return sqrt((xb - xa)*(xb - xa) + (yb - ya)*(yb - ya));
}

bool TextureManagement::ClockWise(const SDL_Point& a, const SDL_Point& b, const SDL_Point& x)
{
    return a.x * (b.y - x.y) + b.x * (x.y - a.y) + x.x * (a.y - b.y) < 0;
}

bool TextureManagement::CounterClockWise(const SDL_Point& a, const SDL_Point& b, const SDL_Point& x)
{
    return a.x * (b.y - x.y) + b.x * (x.y - a.y) + x.x * (a.y - b.y) > 0;
}

bool TextureManagement::IsLineCut(const SDL_Point& a, const SDL_Point& b, const SDL_Point& x, const SDL_Point& y)
{
    /// check if the line connects a and b is cut the line connect x and y
    /// return true if 2 line is cut

    bool cut = true;

    if((!ClockWise(a, b, x) && !CounterClockWise(a, b, x)) || (!ClockWise(a, b, y) && !CounterClockWise(a, b, y)))
    {
        return true;
        /// already cut, form a line
    }
    else
    {
        if(ClockWise(a, b, x) ^ CounterClockWise(a, b, y))
        {
            cut = false;
            return false;
        }
    }

    if((!ClockWise(x, y, a) && !CounterClockWise(x, y, a)) || (!ClockWise(x, y, b) && !CounterClockWise(x, y, b)))
    {
        return true;
        /// already cut, form a line
    }
    else
    {
        if(ClockWise(x, y, a) ^ CounterClockWise(x, y, b))
        {
            cut = false;
            return false;
        }
    }

    return cut;
}
