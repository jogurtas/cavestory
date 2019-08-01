#pragma once

#include <string>
#include "AnimatedSprite.h"
#include "Globals.h"
#include "Player.h"

class Enemy : public AnimatedSprite {
public:
	Enemy();
	
	Enemy(Graphics &gfx, std::string filePath, int srcX, int srcY, 
		int width, int height, Vector2 spawnPoint, int timeToUpdate);

	virtual void update(int deltaTime, Player &player);

	virtual void draw(Graphics &gfx);

	virtual void touchPlayer(Player *player) = 0;

	const inline int getMaxHealth() const { return _maxHealth; }

	const inline int getCurrentHealth() const { return _currentHealth; }

protected:
	Direction _direction;

	int _maxHealth;
	int _currentHealth;
};

class Bat : public Enemy {
public:
	Bat();

	Bat(Graphics &gfx, Vector2 spawnPoint);

	void  update(int deltaTime, Player &player);

	void draw(Graphics &gfx);

	void animationDone(std::string currentAnimation);

	void setupAnimations();

	void touchPlayer(Player *player);

private:
	float _startingX, _startingY;
	bool _shouldMoveUp;
};
