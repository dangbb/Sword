#include "Game.h"

Game::Game(string texture_path)
{
    running = true;
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        running = false;
    }
    else
    {
        Global_Window = SDL_CreateWindow(game_name,
                                         SDL_WINDOWPOS_UNDEFINED,
                                         SDL_WINDOWPOS_UNDEFINED,
                                         SCREEN_WIDTH,
                                         SCREEN_HEIGHT,
                                         SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if(Global_Window == NULL)
        {
            printf("Unable to create window: %s\n", SDL_GetError());
            running = false;
        }
        else
        {
            Ren = SDL_CreateRenderer(Global_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if(Ren == NULL)
            {
                printf("Unable to cerate renderer: %s\n", SDL_GetError());
                running = false;
            }
        }

        int flags = IMG_INIT_PNG;
        if((flags & IMG_Init(flags)) != flags)
        {
            printf("Unable to initialize IMG: $s\n", IMG_GetError());
            running = false;
        }
        if(TTF_Init() < 0)
        {
            printf("Unable to initialize TTF: %s\n", TTF_GetError());
            running = false;
        }
    }

    if(running)
    {
        ifstream read(texture_path.c_str());

        for(int i=0;i<NUM;i++)
        {
            string path;
            read >> path;
            GameTexture::Texture.push_back(TextureManagement::LoadTexture(Ren, path));
        }

        string text_path;

        read >> text_path;
        Text = TTF_OpenFont(text_path.c_str(), 24);

        COMMAND.push_back("NEW GAME");
        COMMAND.push_back("QUIT");

        LOSE.push_back("!!! YOU LOSE !!!");
        LOSE.push_back("TRY AGAIN");
        LOSE.push_back("MENU");
        LOSE.push_back("QUIT");

        WIN.push_back("!!! YOU WIN !!!");
        WIN.push_back("TRY AGAIN");
        WIN.push_back("MENU");
        WIN.push_back("QUIT");

        string level_name;
        while(read >> level_name)
        {
            LEVEL.push_back(level_name);
        }
        LEVEL.push_back("BACK");

        Menu = new GameGUI(Text, COMMAND);
        SelectLevel = new GameGUI(Text, LEVEL);
        Fallen = new GameGUI(Text, LOSE);
        Winner = new GameGUI(Text, WIN);

        read.close();
    }

    Screen = NULL;
}

Game::~Game()
{
    SDL_DestroyWindow(Global_Window);

    delete Menu;
    delete SelectLevel;
    delete Fallen;
    delete Winner;

    if(Screen != NULL) delete Screen;
}

void Game::Run()
{
    int current_level = 0;
    SDL_Event e;

    Screen = NULL;

    bool in_menu = true;
    bool in_choose_level = false;
    bool in_win = false;
    bool in_false = false;
    bool in_game = false;

    while(running)
    {
        if(in_menu)
        {
            Menu->Render(Ren);
            while(SDL_PollEvent(&e))
            {
                if(e.type == SDL_QUIT) running = false;
                int options = Menu->Event(e);

                if(options == 0) /// choose level
                {
                    in_choose_level = true;
                    in_menu = false;
                }
                else if(options == 1) /// quit
                {
                    running = false;
                }
            }
        }
        else if(in_choose_level)
        {
            SelectLevel->Render(Ren);
            while(SDL_PollEvent(&e))
            {
                if(e.type == SDL_QUIT) running = false;
                int options = SelectLevel->Event(e);

                if(options < LEVEL.size())
                {
                    if(options == LEVEL.size() - 1)
                    {
                        in_menu = true;
                        in_choose_level = false;
                    }
                    else
                    {
                        Screen = new GameScreen(Ren, LEVEL[options]);
                        current_level = options;
                        in_choose_level = false;
                        in_game = true;
                    }
                }
            }
        }
        else if(in_game)
        {
            Uint32 GameTime = SDL_GetTicks();

            while(SDL_PollEvent(&e))
            {
                if(e.type == SDL_QUIT) running = false;
                Screen->Event(GameTime, e);
            }

            Screen->Update(GameTime);
            Screen->Loop(GameTime);
            Screen->Render(Ren);

            if(Screen->IsWin())
            {
                in_game = false;
                in_win = true;
                delete Screen;
                Screen = NULL;
            }
            else if(Screen->IsLose())
            {
                in_game = false;
                in_false = true;
                delete Screen;
                Screen = NULL;
            }
        }
        else if(in_win)
        {
            Winner->Render(Ren);
            while(SDL_PollEvent(&e))
            {
                if(e.type == SDL_QUIT) running = false;

                int options = Winner->Event(e);

                if(options == 0) {}
                else if(options == 1)
                {
                    in_win = false;
                    in_game = true;
                    Screen = new GameScreen(Ren, LEVEL[current_level]);
                }
                else if(options == 2)
                {
                    in_win = false;
                    in_menu = true;
                }
                else if(options == 3)
                {
                    in_win = false;
                    running = false;
                }
            }
        }
        else if(in_false)
        {
            Fallen->Render(Ren);
            while(SDL_PollEvent(&e))
            {
                if(e.type == SDL_QUIT) running = false;

                int options = Fallen->Event(e);

                if(options == 0) {}
                else if(options == 1)
                {
                    in_false = false;
                    in_game = true;
                    Screen = new GameScreen(Ren, LEVEL[current_level]);
                }
                else if(options == 2)
                {
                    in_false = false;
                    in_menu = true;
                }
                else if(options == 3)
                {
                    in_false = false;
                    running = false;
                }
            }
        }
    }
}
