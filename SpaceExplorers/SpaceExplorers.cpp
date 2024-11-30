#include "Game.h"
#include <ctime> // For time()


int main() {
    srand(static_cast<unsigned>(time(0))); // Initialize random seed
    Game game;
    game.run();
    return 0;
}
