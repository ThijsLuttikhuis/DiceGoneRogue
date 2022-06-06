//
// Created by thijs on 27-05-22.
//

#include <iostream>

#include "utilities/Random.h"
#include "utilities/Window.h"
#include "GameController.h"

int main() {

    int width = 28*16;
    int height = 28*9;

    DGR::Random::initialize(0);
    auto* window = new DGR::Window(width, height);
    auto* dgrGame = new DGR::GameController(window);
    window->setGameController(dgrGame);

    while (!window->shouldClose()) {
        dgrGame->update();
        window->render();
    }

    return 0;
}