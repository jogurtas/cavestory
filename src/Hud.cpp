#include "Hud.h"
#include "Graphics.h"

#include <cmath>

Hud::Hud() {}

Hud::Hud(Graphics &gfx, Player &player) {
	_player = player;
	_healthBarSprite = Sprite(gfx, "../assets/TextBox.png", 0, 40, 64, 8, 35, 70);
	_healthNumber1 = Sprite(gfx, "../assets/TextBox.png", 0, 56, 8, 8, 66, 70);
	_currentHealthBar = Sprite(gfx, "../assets/TextBox.png", 0, 25, 39, 5, 83, 72);

	_lvWord = Sprite(gfx, "../assets/TextBox.png", 81, 81, 11, 7, 38, 55);
	_lvNumber = Sprite(gfx, "../assets/TextBox.png", 0, 56, 8, 8, 66, 52);
	_expBar = Sprite(gfx, "../assets/TextBox.png", 0, 72, 40, 8, 83, 52);

	_slash = Sprite(gfx, "../assets/TextBox.png", 72, 48, 8, 8, 100, 36);
	_dashes = Sprite(gfx, "../assets/TextBox.png", 81, 51, 15, 11, 132, 26);
}

void Hud::update(int deltaTime, Player &player) {
	_player = player;
	_healthNumber1.setSourceRectX(8 * _player.getCurrentHealth());

	// Calculate the width of health bar
	// 100& = 39px
	float num = (float)_player.getCurrentHealth() / _player.getMaxHealth();
	_currentHealthBar.setSourceRectWidth(std::floor(num * 39));
}

void Hud::draw(Graphics &gfx) {
	_healthBarSprite.draw(gfx, (int)_healthBarSprite.getX(), (int)_healthBarSprite.getY());
	_healthNumber1.draw(gfx, _healthNumber1.getX(), _healthNumber1.getY());
	_currentHealthBar.draw(gfx, _currentHealthBar.getX(), _currentHealthBar.getY());

	_lvWord.draw(gfx, _lvWord.getX(), _lvWord.getY());
	_lvNumber.draw(gfx, _lvNumber.getX(), _lvNumber.getY());
	_expBar.draw(gfx, _expBar.getX(), _expBar.getY());

	_slash.draw(gfx, _slash.getX(), _slash.getY());
	_dashes.draw(gfx, _dashes.getX(), _dashes.getY());
}
