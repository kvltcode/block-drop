#include "Game.h"

int main(void)
{
    Game game;
    bool run = true;

    while (run)
    {
        run = game.poll();
        game.update();
        game.draw();
    }

    return 0;
}