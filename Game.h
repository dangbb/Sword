#ifndef _GAME_H_
#define _GAME_H_

#include "header/GameOptions.h"
#include "header/GameAnimate.h"
#include "header/GameSprite.h"
#include "header/GamePlayer.h"
#include "header/GameEntity.h"
#include "header/GameScreen.h"
#include "header/GameSound.h"
#include "header/GameGUI.h"

class Game
{
    public:
        Game(string texture_path);
        ~Game();

        void Run();
    private:

        void Event();
        void Update();
        void Render(SDL_Renderer* Ren);

        bool running;

        SDL_Renderer* Ren;
        SDL_Window* Global_Window;

        GameScreen* Screen;
        GameGUI* Menu;
        GameGUI* SelectLevel;
        GameGUI* Fallen;
        GameGUI* Winner;

        TTF_Font* Text;

        vector<string> COMMAND;
        vector<string> LEVEL;
        vector<string> LOSE;
        vector<string> WIN;
};

#endif // _GAME_H_


