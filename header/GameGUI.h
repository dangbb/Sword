#ifndef _GAME_GUI_H_
#define _GAME_GUI_H_

#include "GameOptions.h"

class GameGUI
{
    public:
        GameGUI(TTF_Font* Text, vector<string> S);
        ~GameGUI();

        SDL_Texture* GetText(SDL_Renderer* Ren, string S);
        void Render(SDL_Renderer* Ren);
        int Event(SDL_Event e);

    private:
        TTF_Font* Text;

        vector<string> Options;
};

#endif // _GAME_GUI_H_

