#pragma once

#include "AnimatedSprite.h"
#include "Globals.h"

class Grapics;

class Player : public AnimatedSprite {
public:
	Player();

	Player(Graphics &gfx, float x, float y);

	void draw(Graphics &gfx);

	void update(float deltaTime);

	virtual void animationDone(std::string currentAnimation);

	virtual void setupAnimations();

	/// Moves the player by -dx
	void moveLeft();

	/// Moves the player by dx
	void moveRight();

	/// Stops moving the player
	void stopMoving();
private:
	float _dx, _dy;

	Direction _facing;
};