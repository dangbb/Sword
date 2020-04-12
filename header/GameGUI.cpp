#include "GameGUI.h"

GameGUI::GameGUI(TTF_Font* Text, vector<string> S)
{
    this->Text = Text;
    this->Options = S;
}

GameGUI::~GameGUI()
{

}

void GameGUI::Render(SDL_Renderer* Ren)
{
    SDL_SetRenderDrawColor(Ren, 0, 0, 0, 0);
    SDL_RenderClear(Ren);
    int pos_x;
    int pos_y;

    pos_x = SCREEN_WIDTH / 2;
    pos_y = SCREEN_HEIGHT / 2 - 30;

    int mouse_pos_x;
    int mouse_pos_y;

    SDL_GetMouseState(&mouse_pos_x, &mouse_pos_y);

    for(int i=0;i<Options.size();i++)
    {
        SDL_Rect Border = {pos_x - 150, pos_y - 15, 300, 30};

        SDL_SetRenderDrawColor(Ren, 0, 0, 0, 0);
        SDL_RenderFillRect(Ren, &Border);
        SDL_SetRenderDrawColor(Ren, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderDrawRect(Ren, &Border);

        if(Border.x <= mouse_pos_x && mouse_pos_x <= Border.x + Border.w)
        {
            if(Border.y <= mouse_pos_y && mouse_pos_y <= Border.y + Border.h)
            {
                Border.x -= 2;
                Border.y -= 2;
                Border.w += 4;
                Border.h += 4;

                SDL_RenderDrawRect(Ren, &Border);
            }
        }

        SDL_Texture* TextTexture = GetText(Ren, Options[i].c_str());
        int width;
        int height;
        SDL_QueryTexture(TextTexture, NULL, NULL, &width, &height);
        SDL_Rect Name = {pos_x - width / 2, pos_y - height / 2, width, height};
        SDL_RenderCopy(Ren, TextTexture, NULL, &Name);
        pos_y += 31;
        SDL_DestroyTexture(TextTexture);
    }
    SDL_RenderPresent(Ren);
}

int GameGUI::Event(SDL_Event e)
{
    if(e.type == SDL_MOUSEBUTTONDOWN)
    {
        int pos_x;
        int pos_y;

        pos_x = SCREEN_WIDTH / 2;
        pos_y = SCREEN_HEIGHT / 2 - 30;

        int mouse_pos_x;
        int mouse_pos_y;

        SDL_GetMouseState(&mouse_pos_x, &mouse_pos_y);

        for(int i=0;i<Options.size();i++)
        {
            SDL_Rect Border = {pos_x - 150, pos_y - 15, 300, 30};

            if(Border.x <= mouse_pos_x && mouse_pos_x <= Border.x + Border.w)
            {
                if(Border.y <= mouse_pos_y && mouse_pos_y <= Border.y + Border.h)
                {
                    return i;
                }
            }
            pos_y += 31;
        }
    }
    return 100;
}

SDL_Texture* GameGUI::GetText(SDL_Renderer* Ren, string s)
{
    SDL_Surface* TextSurface = TTF_RenderText_Blended(this->Text, s.c_str(), SDL_Color{0xFF, 0xFF, 0xFF, 0xFF});

    SDL_Texture* TextTexture = SDL_CreateTextureFromSurface(Ren, TextSurface);

    SDL_FreeSurface(TextSurface);

    return TextTexture;
}
