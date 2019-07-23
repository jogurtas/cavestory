#pragma once

#include "AnimatedSprite.h"

class Graphics;

/// This class holds all information for main game loop
class Game {
public:
    Game();

    ~Game();

private:
    AnimatedSprite player;

    void gameLoop();

    void draw(Graphics &graphics);

    void update(float deltaTime);
};
