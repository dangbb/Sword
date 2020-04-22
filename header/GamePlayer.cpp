#include "GamePlayer.h"

bool GamePlayer::SortPolicy(const GamePlayer* PlayerA, const GamePlayer* PlayerB)
{
    int pos_ax, pos_ay;
    int pos_bx, pos_by;

    PlayerA->GetPos(pos_ax, pos_ay);
    PlayerB->GetPos(pos_bx, pos_by);

    return pos_ay < pos_by;
}

GamePlayer::GamePlayer(const int& player_id, const int& init_pos_x, const int& init_pos_y)
{
    this->id = player_id;

    SDL_QueryTexture(GameTexture::Texture[CUT_SPRITE], NULL, NULL, &width, &height);
    cut_animation = new GameAnimation(width, height, width / 2, height);
    cut_animation->SetMode();

    burn = false;
    SDL_QueryTexture(GameTexture::Texture[BURN_SYM], NULL, NULL, &width, &height);
    burn_animation = new GameAnimation(width, height, width, height);
    burn_animation->SetMode();

    alert_animation = new GameAnimation(1,1,1,1);
    alert_animation->SetMode();

    reload_animation = new GameAnimation(1,1,1,1);
    reload_animation->SetMode();

    delay_fire_animation = new GameAnimation(1,1,1,1);
    delay_fire_animation->SetMode();

    SDL_QueryTexture(GameTexture::Texture[id], NULL, NULL, &width, &height);

    player_physics = new GamePhysics(init_pos_x, init_pos_y);

    Dest.x = init_pos_x - width / 2;
    Dest.y = init_pos_y - height / 2;
    Dest.w = width;
    Dest.h = height;

    FLIP = SDL_FLIP_NONE;

    alert = false;
    attack = false;
    attack_radian = 0.0;

    max_health = 30;
    cur_health = 30;

    angle = 0.0;
    burn = false;

    dead = false;
    complete_death = false;

    properties = GET_DAME | BLOCK_BULLET;

    if(id == GUN_BOT)
    {
        bullet = 3;
    }
    else if(id == FLAME_BOT)
    {
        bullet = 10;
    }

    IsLeft = USABLE;
    IsRight = USABLE;
    IsUp = USABLE;
    IsDown = USABLE;
}

GamePlayer::~GamePlayer()
{
    delete player_physics;
    if(id == PLAYER)
    {
        delete cut_animation;
    }
}

bool GamePlayer::Collision(const GameSprite* const e)
{
    if(cur_health < 0)
    {
        dead = true;
    }
    if(dead) return false;

    SDL_Rect HitBox;
    e->GetHitbox(HitBox);

    return this->player_physics->Collision(HitBox, this->id, this->width / 2);
}

void GamePlayer::Event(const Uint32& GameTime, SDL_Event& e)
{
    if(cur_health < 0)
    {
        dead = true;
    }
    if(dead) return;

    const Uint8* KeyboardState = SDL_GetKeyboardState(NULL);

    if(KeyboardState[SDL_SCANCODE_W] && IsUp == USABLE)
    {
        IsUp = INUSE;
        player_physics->SetAy(-3000);
        player_physics->SetLowerVy(-300);
        if(IsDown == INUSE)
        {
            IsDown = DISABLE;
        }
    }
    if(KeyboardState[SDL_SCANCODE_D] && IsRight == USABLE)
    {
        IsRight = INUSE;
        player_physics->SetAx(+3000);
        player_physics->SetUpperVx(+300);
        if(IsLeft == INUSE)
        {
            IsLeft = DISABLE;
        }
    }
    if(KeyboardState[SDL_SCANCODE_S] && IsDown == USABLE)
    {
        IsDown = INUSE;
        player_physics->SetAy(+3000);
        player_physics->SetUpperVy(+300);
        if(IsUp == INUSE)
        {
            IsUp = DISABLE;
        }
    }
    if(KeyboardState[SDL_SCANCODE_A] && IsLeft == USABLE)
    {
        IsLeft = INUSE;
        player_physics->SetAx(-3000);
        player_physics->SetLowerVx(-300);
        if(IsRight == INUSE)
        {
            IsRight = DISABLE;
        }
    }

    if(e.type == SDL_MOUSEBUTTONDOWN)
    {
        int mouse_pos_x;
        int mouse_pos_y;

        SDL_GetMouseState(&mouse_pos_x, &mouse_pos_y);

        int pos_x;
        int pos_y;

        GetPos(pos_x, pos_y);

        double moving_angle = atan2(mouse_pos_y - pos_y + ESL, mouse_pos_x - pos_x);

        if(e.button.button == SDL_BUTTON_LEFT)
        {
            if(id == PLAYER && !attack)
            {
                Mix_HaltChannel(this->channel_id);
                Mix_PlayChannel(this->channel_id, SoundLib::SoundEffect[SE_CUT], 0);
                attack = true;
                cut_animation->SetMode(0, GameTime, 20, 0, ONEWAY);

                if(FLIP == SDL_FLIP_NONE)
                {
                    FLIP = SDL_FLIP_VERTICAL;
                }
                else
                {
                    FLIP = SDL_FLIP_NONE;
                }
            }
        }
    }
    else if(e.type == SDL_KEYUP)
    {
        switch(e.key.keysym.sym)
        {
            case SDLK_w:
                if(IsUp == INUSE)
                {
                    player_physics->SetAy(+3000);
                    player_physics->SetUpperVy(0);
                }
                IsUp = USABLE;
                break;

            case SDLK_d:
                if(IsRight == INUSE)
                {
                    player_physics->SetAx(-3000);
                    player_physics->SetLowerVx(0);
                }
                IsRight = USABLE;
                break;

            case SDLK_a:
                if(IsLeft == INUSE)
                {
                    player_physics->SetAx(+3000);
                    player_physics->SetUpperVx(0);
                }
                IsLeft = USABLE;
                break;

            case SDLK_s:
                if(IsDown == INUSE)
                {
                    player_physics->SetAy(-3000);
                    player_physics->SetLowerVy(0);
                }
                IsDown = USABLE;
                break;
        }
    }
}

void GamePlayer::GetDame(const Uint32& GameTime, const int& id, const int& properties)
{
    if(cur_health < 0)
    {
        dead = true;
    }
    if(dead) return;
    if(properties & FROM_ENEMY && (this->id == FLAME_BOT || this->id == GUN_BOT)) return;
    if(properties & FROM_PLAYER && (this->id == PLAYER)) return;

    if(id == FLAME_BULLET)
    {
        if(burn)
        {
            Uint32 LastTimeBurn;
            burn_animation->GetTimer(LastTimeBurn);

            if(GameTime - LastTimeBurn >= 1000)
            {
                burn_animation->SetMode(0, GameTime, 5000, 0, ONEWAY);
                cur_health -= 7;
            }
        }
        else
        {
            cur_health -= 7;
            burn = true;
            burn_animation->SetMode(0, GameTime, 5000, 0, ONEWAY);
        }
    }

    if(id == CUT_SPRITE) cur_health -= 30;
    if(id == GUN_BULLET) cur_health -= 7;
}

void GamePlayer::GetPos(int& pos_x, int& pos_y) const
{
    player_physics->GetPos(pos_x, pos_y);
}

void GamePlayer::GetRadian(int &r) const
{
    r = this->width / 2;
}

void GamePlayer::GetProperties(int& properties) const
{
    properties = this->properties;
}

bool GamePlayer::IsAttack(double& angle)
{
    if(cur_health < 0)
    {
        dead = true;
    }
    if(!attack) return false;
    if(dead) return false;

    angle = this->angle;

    int state;
    this->cut_animation->GetState(state);

    if(state != 1)
    {
        return false;
    }

    return true;
}

bool GamePlayer::IsBurn()
{
    return this->burn;
}

bool GamePlayer::IsDeath()
{
    if(cur_health < 0)
    {
        dead = true;
    }
    return this->dead;
}

void GamePlayer::Render(SDL_Renderer* Ren, const int& screen_pos_x, const int& screen_pos_y)
{
    if(cur_health < 0)
    {
        dead = true;
    }
    if(dead) return;

    int pos_x;
    int pos_y;
    player_physics->GetPos(pos_x, pos_y);

    int mouse_pos_x;
    int mouse_pos_y;
    SDL_GetMouseState(&mouse_pos_x, &mouse_pos_y);

    double perspective_angle = atan2(mouse_pos_y - (pos_y - screen_pos_y) + ESL, mouse_pos_x - (pos_x - screen_pos_x));

    if(id == PLAYER && !attack) angle = perspective_angle;

    Dest.x = pos_x - width / 2 - screen_pos_x;
    Dest.y = pos_y - height / 2 - screen_pos_y;
    Dest.w = width;
    Dest.h = height;

    if(Dest.x > SCREEN_WIDTH || Dest.x + Dest.w < 0 || Dest.y > SCREEN_HEIGHT || Dest.y + Dest.h < 0)
    {

    }
    else
    {
        SDL_RenderCopyEx(Ren, GameTexture::Texture[id], NULL, &Dest, angle * 180 / PI, NULL, SDL_FLIP_NONE);
    }

    if(attack)
    {
        SDL_Rect Source;
        cut_animation->Source(Source);

        Dest = {pos_x - Source.w / 2 - screen_pos_x, pos_y - Source.h / 2 - screen_pos_y, Source.w, Source.h};
        SDL_RenderCopyEx(Ren, GameTexture::Texture[CUT_SPRITE], &Source, &Dest, angle * 180 / PI, NULL, FLIP);
    }

    if(id == PLAYER)
    {
        SDL_Rect Dest = {SCREEN_WIDTH - max_health * 5 - 10, 10, max_health * 5, 30};
        SDL_SetRenderDrawColor(Ren, 255, 0, 0, 0);
        SDL_RenderFillRect(Ren, &Dest);

        Dest.w = cur_health * 5;
        SDL_SetRenderDrawColor(Ren, 0, 255, 0, 0);
        SDL_RenderFillRect(Ren, &Dest);

        Dest.x -= 40;
        Dest.w = 30;
        Dest.h = 30;

        SDL_RenderCopy(Ren, GameTexture::Texture[HEALTH_SYM], NULL, &Dest);

        if(burn)
        {
            Dest.x -= 40;
            SDL_RenderCopy(Ren, GameTexture::Texture[BURN_SYM], NULL, &Dest);
        }
    }
}

void GamePlayer::Update(const Uint32& GameTime)
{
    if(cur_health < 0)
    {
        dead = true;
    }
    if(dead) return;

    if(id == PLAYER)
    {
        const Uint8* KeyboardState = SDL_GetKeyboardState(NULL);

        if(KeyboardState[SDL_SCANCODE_W] && IsUp == USABLE)
        {
            IsUp = INUSE;
            player_physics->SetAy(-3000);
            player_physics->SetLowerVy(-300);
            if(IsDown == INUSE)
            {
                IsDown = DISABLE;
            }
        }
        if(KeyboardState[SDL_SCANCODE_D] && IsRight == USABLE)
        {
            IsRight = INUSE;
            player_physics->SetAx(+3000);
            player_physics->SetUpperVx(+300);
            if(IsLeft == INUSE)
            {
                IsLeft = DISABLE;
            }
        }
        if(KeyboardState[SDL_SCANCODE_S] && IsDown == USABLE)
        {
            IsDown = INUSE;
            player_physics->SetAy(+3000);
            player_physics->SetUpperVy(+300);
            if(IsUp == INUSE)
            {
                IsUp = DISABLE;
            }
        }
        if(KeyboardState[SDL_SCANCODE_A] && IsLeft == USABLE)
        {
            IsLeft = INUSE;
            player_physics->SetAx(-3000);
            player_physics->SetLowerVx(-300);
            if(IsRight == INUSE)
            {
                IsRight = DISABLE;
            }
        }
    }
    cut_animation->Update(GameTime);
    alert_animation->Update(GameTime);
    burn_animation->Update(GameTime);
    player_physics->Update(GameTime);

    reload_animation->Update(GameTime);
    delay_fire_animation->Update(GameTime);

    int state;
    cut_animation->GetState(state);

    if(state == 2)
    {
        attack = false;
    }

    burn_animation->GetState(state);
    if(state == 1)
    {
        burn = false;
    }
}

void GamePlayer::BotBehaviour(const Uint32& GameTime, const double& r, const double& distance, bool seen)
{
    if(alert)
    {
        int state;
        this->alert_animation->GetState(state);

        if(state == 1)
        {
            alert = false;
        }
    }
    if(!seen)
    {
        this->player_physics->SetLowerVx(0);
        this->player_physics->SetUpperVx(0);
        this->player_physics->SetLowerVy(0);
        this->player_physics->SetUpperVy(0);
    }
    else
    {
        alert = true;
        this->alert_animation->SetMode(0, GameTime, 5000, 0, ONEWAY);
        if(this->id == FLAME_BOT)
        {
            if(distance < 350)
            {
                this->player_physics->SetAx(-1000 * cos(r));
                this->player_physics->SetAy(-1000 * sin(r));
                this->player_physics->SetLowerVy(-100);
                this->player_physics->SetLowerVx(-100);
                this->player_physics->SetUpperVy(+100);
                this->player_physics->SetUpperVx(+100);
            }
            else if(350 <= distance && distance <= 400)
            {
                this->player_physics->SetAx(+3000 * cos(r));
                this->player_physics->SetAy(+3000 * sin(r));
                this->player_physics->SetLowerVy(-100);
                this->player_physics->SetLowerVx(-100);
                this->player_physics->SetUpperVy(+100);
                this->player_physics->SetUpperVx(+100);
            }
        }
        else
        {
            if(distance < 700)
            {
                this->player_physics->SetAx(-3000 * cos(r));
                this->player_physics->SetAy(-3000 * sin(r));
                this->player_physics->SetLowerVy(-100);
                this->player_physics->SetLowerVx(-100);
                this->player_physics->SetUpperVy(+100);
                this->player_physics->SetUpperVx(+100);
            }
            else
            {
                this->player_physics->SetAx(+3000 * cos(r));
                this->player_physics->SetAy(+3000 * sin(r));
                this->player_physics->SetLowerVy(-100);
                this->player_physics->SetLowerVx(-100);
                this->player_physics->SetUpperVy(+100);
                this->player_physics->SetUpperVx(+100);
            }
        }
        /// ************ FIRE
        int state;
        int pos_x;
        int pos_y;
        this->player_physics->GetPos(pos_x, pos_y);

        if(bullet == 0)
        {
            reload_animation->GetState(state);
            if(state == 1)
            {
                if(id == GUN_BOT)
                    bullet = 3;
                else if(id == FLAME_BOT)
                    bullet = 10;
            }
        }
        if(bullet)
        {
            delay_fire_animation->GetState(state);
            if(state == 1)
            {
                --bullet;
                if(this->id == FLAME_BOT)
                {
                    int properties = BURN | DEAL_DAME | FROM_ENEMY | PROGRESSIVE_GROW;
                    new GameEntity(FLAME_BULLET, pos_x, pos_y, r, properties);

                    delay_fire_animation->SetMode(0, GameTime, 50, 0, ONEWAY);

                    Mix_HaltChannel(this->channel_id);
                    Mix_PlayChannelTimed(this->channel_id, SoundLib::SoundEffect[SE_FLAMESHOT], 0, 2000);
                }
                else if(this->id == GUN_BOT)
                {
                    int properties = DEAL_DAME | FROM_ENEMY;
                    new GameEntity(GUN_BULLET, pos_x, pos_y, r + (rand() % 10 - rand() % 10) * PI / 180, properties);

                    delay_fire_animation->SetMode(0, GameTime, 50, 0, ONEWAY);

                    Mix_HaltChannel(this->channel_id);
                    Mix_PlayChannel(this->channel_id, SoundLib::SoundEffect[SE_GUNSHOT], 0);
                }
                if(bullet == 0)
                {
                    reload_animation->SetMode(0, GameTime, 1000, 0, ONEWAY);
                }
            }
        }
    }
}

void GamePlayer::SetChannel(int new_channel)
{
    this->channel_id = new_channel;
}


