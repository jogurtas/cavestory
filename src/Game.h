#pragma once

#include "Player.h"
#include "Level.h"
#include "Hud.h"
#include "Graphics.h"

/// This class holds all information for main game loop
class Game {
public:
    Game();

    ~Game();

private:
    Player player;
	Level level;
	Hud hud;
	Graphics _gfx;

    void gameLoop();

    void draw(Graphics &graphics);

    void update(float deltaTime);
};
