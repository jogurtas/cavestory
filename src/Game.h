#pragma once

#include "Player.h"
#include "Level.h"

class Graphics;

/// This class holds all information for main game loop
class Game {
public:
    Game();

    ~Game();

private:
    Player player;
	Level level;

    void gameLoop();

    void draw(Graphics &graphics);

    void update(float deltaTime);
};
