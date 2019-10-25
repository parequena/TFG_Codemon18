#include <Game.hpp>

int main()
{
    Game* g = new Game();
    g->play();
    delete g;
    return 0;
}