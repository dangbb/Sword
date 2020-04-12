#include "GameScreen.h"

GameScreen::GameScreen(SDL_Renderer* Ren, string screen_path)
{
    srand( time(NULL) );
    ifstream read(screen_path.c_str());

    {
        int number_barrel;
        read >> number_barrel;

        for(int i=0;i<number_barrel;i++)
        {
            int pos_x;
            int pos_y;
            read >> pos_x >> pos_y;

            Obstacle.push_back(new GameSprite(BARREL, pos_x, pos_y));
        }
    }

    {
        int number_wall;
        read >> number_wall;

        for(int i=0;i<number_wall;i++)
        {
            int pos_x;
            int pos_y;
            read >> pos_x >> pos_y;

            Obstacle.push_back(new GameSprite(WALL, pos_x, pos_y));
        }
    }

    {
        int number_flame_bot;
        read >> number_flame_bot;

        for(int i=0;i<number_flame_bot;i++)
        {
            int pos_x;
            int pos_y;
            read >> pos_x >> pos_y;

            Enemy.push_back(new GamePlayer(FLAME_BOT, pos_x, pos_y));
        }
    }

    {
        int number_gun_bot;
        read >> number_gun_bot;

        for(int i=0;i<number_gun_bot;i++)
        {
            int pos_x;
            int pos_y;
            read >> pos_x >> pos_y;

            Enemy.push_back(new GamePlayer(GUN_BOT, pos_x, pos_y));
        }
    }

    {
        int pos_x;
        int pos_y;
        read >> pos_x >> pos_y;

        MainPlayer = new GamePlayer(PLAYER, pos_x, pos_y);
    }

    sort(Obstacle.begin(), Obstacle.end(), GameSprite::SortPolicy);

    running = true;
    read.close();
}

GameScreen::~GameScreen()
{
    for(int i=0;i<Enemy.size();i++)
    {
        delete Enemy[i];
    }
    for(int i=0;i<Obstacle.size();i++)
    {
        delete Obstacle[i];
    }
    delete MainPlayer;

    Enemy.clear();
    Obstacle.clear();
}

void GameScreen::Event(const Uint32& GameTime, SDL_Event e)
{
    MainPlayer->Event(GameTime, e);
}

void GameScreen::Update(const Uint32& GameTime)
{
    GameEntity::flame->GetNext()->Update(GameTime);
    GameEntity::bullet->GetNext()->Update(GameTime);
    /// Enemy[i]->Update(GameTime);

    for(int i=0;i<Enemy.size();i++)
    {
        Enemy[i]->Update(GameTime);
    }
    MainPlayer->Update(GameTime);
}

void GameScreen::Loop(const Uint32& GameTime)
{
    int obstacle_id = 0;
    MainPlayer->Collision(Obstacle[obstacle_id]);

    for(int i=0;i<Enemy.size();i++)
    {
        for(int j=0;j<Obstacle.size();j++)
        {
            Enemy[i]->Collision(Obstacle[j]);
        }
    }

    for(int i=0;i<Obstacle.size();i++)
    {
        MainPlayer->Collision(Obstacle[i]);
    }

    int main_pos_x;
    int main_pos_y;
    int main_radius;
    int main_properties;

    MainPlayer->GetRadian(main_radius);
    MainPlayer->GetPos(main_pos_x, main_pos_y);
    MainPlayer->GetProperties(main_properties);

    if(MainPlayer->IsDeath())
    {
        main_pos_x = -100000000;
        main_pos_y = -100000000;
        /// bot stop attack
    }

    GameEntity* bullet = GameEntity::bullet->GetNext();
    GameEntity* next;

    double angle;
    if(MainPlayer->IsAttack(angle))
    {
        int hit_width;
        SDL_QueryTexture(GameTexture::Texture[CUT_SPRITE], NULL, NULL, NULL, &hit_width);

        int id = CUT_SPRITE;
        int properties = DEAL_DAME | FROM_PLAYER;

        double lower_angle = angle - PI / 2;
        double upper_angle = angle + PI / 2;

        for(int i=0;i<Enemy.size();i++)
        {
            int pos_x;
            int pos_y;
            int width;
            Enemy[i]->GetPos(pos_x, pos_y);
            Enemy[i]->GetRadian(width);

            if(TextureManagement::Distance(pos_x, main_pos_x, pos_y, main_pos_y) <= width + hit_width)
            {
                double enemy_angle = atan2(pos_y - main_pos_y + ESL, pos_x - main_pos_x);
                if(lower_angle <= enemy_angle && enemy_angle <= upper_angle)
                {
                    Enemy[i]->GetDame(GameTime, id, properties);
                }
                else if(lower_angle <= enemy_angle + 2 * PI && enemy_angle + 2 * PI <= upper_angle)
                {
                    Enemy[i]->GetDame(GameTime, id, properties);
                }
                else if(lower_angle <= enemy_angle - 2 * PI && enemy_angle - 2 * PI <= upper_angle)
                {
                    Enemy[i]->GetDame(GameTime, id, properties);
                }
            }
        }

        while(bullet != NULL)
        {
            next = bullet->GetNext();

            int id;
            int properties;
            bullet->GetStatus(id, properties);

            if(id == GUN_BULLET)
            {
                bullet->Collision(SDL_Point{main_pos_x, main_pos_y}, hit_width / 2, main_properties);
            }

            if(next == NULL) break;
            bullet = next;
        }
    }

    bullet = GameEntity::bullet->GetNext();

    while(bullet != NULL)
    {
        next = bullet->GetNext();

        int id;
        int properties;
        bullet->GetStatus(id, properties);

        if(bullet->Collision(SDL_Point{main_pos_x, main_pos_y}, main_radius, main_properties))
        {
            MainPlayer->GetDame(GameTime, id, properties);
        }
        else
        {
            for(int i=0;i<Obstacle.size();i++)
            {
                SDL_Rect r;
                int obstacle_properties;

                Obstacle[i]->GetHitbox(r);
                Obstacle[i]->GetProperties(obstacle_properties);

                if(bullet->Collision(r, obstacle_properties))
                {
                    break;
                }
            }
        }

        if(next == NULL) break;
        bullet = next;
    }

    bullet = GameEntity::flame->GetNext();

    while(bullet != NULL)
    {
        next = bullet->GetNext();

        int id;
        int properties;
        bullet->GetStatus(id, properties);

        if(bullet->Collision(SDL_Point{main_pos_x, main_pos_y}, main_radius, main_properties))
        {
            MainPlayer->GetDame(GameTime, id, properties);
        }
        else
        {
            for(int i=0;i<Obstacle.size();i++)
            {
                SDL_Rect r;
                int obstacle_properties;

                Obstacle[i]->GetHitbox(r);
                Obstacle[i]->GetProperties(obstacle_properties);

                if(bullet->Collision(r, obstacle_properties))
                {
                    break;
                }
            }
        }

        if(next == NULL) break;
        bullet = next;
    }

    vector <SDL_Point> MainPlayerPoints;

    for(int i=0;i<6;i++)
    {
        MainPlayerPoints.push_back(SDL_Point{int(main_pos_x + main_radius * cos(Angle_Instance[i])),
                                   int(main_pos_y + main_radius * sin(Angle_Instance[i]))});
    }

    for(int i=0;i<Enemy.size();i++)
    {
        int pos_x;
        int pos_y;
        Enemy[i]->GetPos(pos_x, pos_y);

        bool seen = false;

        for(int k=0;k<6 && !seen;k++)
        {
            bool current_viewpoint_block = false;

            for(int j=0;j<Obstacle.size() && !current_viewpoint_block;j++)
            {
                SDL_Rect e;
                Obstacle[j]->GetHitbox(e);

                if(TextureManagement::IsLineCut(MainPlayerPoints[k], SDL_Point{pos_x, pos_y}, SDL_Point{e.x, e.y}, SDL_Point{e.x + e.w, e.y}))
                {
                    current_viewpoint_block = true;
                }
                else if(TextureManagement::IsLineCut(MainPlayerPoints[k], SDL_Point{pos_x, pos_y}, SDL_Point{e.x + e.w, e.y}, SDL_Point{e.x + e.w, e.y + e.h}))
                {
                    current_viewpoint_block = true;
                }
                else if(TextureManagement::IsLineCut(MainPlayerPoints[k], SDL_Point{pos_x, pos_y}, SDL_Point{e.x + e.w, e.y + e.h}, SDL_Point{e.x, e.y + e.h}))
                {
                    current_viewpoint_block = true;
                }
                else if(TextureManagement::IsLineCut(MainPlayerPoints[k], SDL_Point{pos_x, pos_y}, SDL_Point{e.x, e.y + e.h}, SDL_Point{e.x, e.y}))
                {
                    current_viewpoint_block = true;
                }
                else if(TextureManagement::IsLineCut(MainPlayerPoints[k], SDL_Point{pos_x, pos_y}, SDL_Point{e.x, e.y}, SDL_Point{e.x + e.w, e.y + e.h}))
                {
                    current_viewpoint_block = true;
                }
                else if(TextureManagement::IsLineCut(MainPlayerPoints[k], SDL_Point{pos_x, pos_y}, SDL_Point{e.x + e.w, e.y}, SDL_Point{e.x, e.y + e.h}))
                {
                    current_viewpoint_block = true;
                }
            }

            if(!current_viewpoint_block)
            {
                seen = true;
                break;
            }
        }

        double r = atan2(main_pos_y - pos_y + ESL, main_pos_x - pos_x);
        Enemy[i]->BotBehaviour(GameTime, r, TextureManagement::Distance(main_pos_y, pos_y, main_pos_x, pos_x), seen);
    }
}

void GameScreen::Render(SDL_Renderer* Ren)
{
    SDL_SetRenderDrawColor(Ren, 0, 0, 0, 0);
    SDL_RenderClear(Ren);

    int screen_pos_x;
    int screen_pos_y;

    MainPlayer->GetPos(screen_pos_x, screen_pos_y);

    screen_pos_x -= SCREEN_WIDTH / 2;
    screen_pos_y -= SCREEN_HEIGHT / 2;

    for(int i=0;i<Obstacle.size();i++)
    {
        Obstacle[i]->Render(Ren, screen_pos_x, screen_pos_y);
    }

    GameEntity::flame->GetNext()->Render(Ren, screen_pos_x, screen_pos_y);
    GameEntity::bullet->GetNext()->Render(Ren, screen_pos_x, screen_pos_y);

    for(int i=0;i<Enemy.size();i++)
    {
        Enemy[i]->Render(Ren, screen_pos_x, screen_pos_y);
    }

    MainPlayer->Render(Ren, screen_pos_x, screen_pos_y);



    SDL_RenderPresent(Ren);
}

bool GameScreen::IsLose()
{
    return MainPlayer->IsDeath();
}

bool GameScreen::IsWin()
{
    bool win = true;

    for(int i=0;i<Enemy.size();i++)
    {
        if(!Enemy[i]->IsDeath())
        {
            win = false;
            break;
        }
    }
    return win;
}

