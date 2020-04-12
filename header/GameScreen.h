#ifndef _GAME_SCREEN_H_
#define _GAME_SCREEN_H_

#include "GameOptions.h"
#include "GamePlayer.h"
#include "GameSprite.h"
#include "GameEntity.h"

class GameScreen
{
    public:
        GameScreen(SDL_Renderer* Ren, string screen_path);
        ~GameScreen();

        void Event(const Uint32& GameTime, SDL_Event e);
        void Loop(const Uint32& GameTime);
        void Update(const Uint32& GameTime);
        void Render(SDL_Renderer* Ren);

        bool IsWin();
        bool IsLose();

    private:
        bool running;

        vector<GamePlayer*> Enemy;
        vector<GameSprite*> Obstacle;

        GamePlayer* MainPlayer;
};

#endif // _GAME_SCREEN_H_
