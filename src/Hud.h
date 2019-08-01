#pragma once

#include "Sprite.h"
#include "Player.h"

class Graphics;
class Player;

class Hud {
public:
	Hud();

	Hud(Graphics &gfx, Player &player);

	void update(int deltaTime, Player &player);

	void draw(Graphics &gfx);

private:
	Player _player;

	// Health sprites
	Sprite _healthBarSprite;
	Sprite _healthNumber1;
	Sprite _currentHealthBar;

	// Exp sprites
	Sprite _lvWord;
	Sprite _lvNumber;
	Sprite _expBar;

	// Weapon info
	Sprite _slash;
	Sprite _dashes;
};