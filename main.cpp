#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <iomanip>

#include "controller.h"

using namespace std;


int main() {
    srand(time(0));

    Game game(rand());
    game.main();

    return 0;
}
