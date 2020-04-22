
#include "Game.h"

int main(int argc, char *argv[])
{
    Game* Swordman = new Game("anno/game_path.txt");

    Swordman->Run();

    delete Swordman;

    return 0;
}
