#ifndef _GAME_PLAYER_H_
#define _GAME_PLAYER_H_

#include "GameOptions.h"
#include "GamePhysics.h"
#include "GameAnimate.h"
#include "GameSprite.h"
#include "GameEntity.h"

class GamePlayer
{
    public:
        static bool SortPolicy(const GamePlayer* PlayerA, const GamePlayer* PlayerB);
        GamePlayer(const int& player_id, const int& init_pos_x, const int& init_pos_y);
        ~GamePlayer();

        void Render(SDL_Renderer* Ren, const int& screen_pos_x, const int& screen_pos_y);
        void Update(const Uint32& GameTime); /// update for player
        void BotBehaviour(const Uint32& GameTime, const double& radius, const double& distance, bool seen); /// strategies for BOT

        void Event(const Uint32& GameTime, SDL_Event& e); /// only use for main player
        void GetPos(int &pos_x, int &pos_y) const;
        void GetRadian(int &r) const;
        void GetDame(const Uint32& GameTime, const int& id, const int& properties);
        void GetProperties(int& properties) const;

        bool Collision(const GameSprite* const e);
        bool IsAttack(double& angle);
        bool IsBurn();
        bool IsDeath();

    protected:
        GamePhysics* player_physics;
        GameAnimation* cut_animation;
        GameAnimation* fire_animation;
        GameAnimation* burn_animation;
        GameAnimation* alert_animation;

        GameAnimation* reload_animation;
        GameAnimation* delay_fire_animation;

        int id;
        int properties;

        SDL_RendererFlip FLIP;
        SDL_Rect Dest;

        double attack_radian;
        double angle;

        bool attack;
        bool burn;
        bool alert;

        int cur_health;
        int max_health;

        /// BOT
        int bullet;

        int width;
        int height;

        int death_counter;
        int visible_counter;
        bool visible;
        bool dead;

        bool complete_death;

        DIRECTION_STATE IsUp;
        DIRECTION_STATE IsDown;
        DIRECTION_STATE IsLeft;
        DIRECTION_STATE IsRight;
};

#endif // _GAMEPLAYER_H_
