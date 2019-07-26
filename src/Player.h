#pragma once

#include "AnimatedSprite.h"
#include "Globals.h"

class Grapics;

class Player : public AnimatedSprite {
public:
	Player();

	Player(Graphics &gfx, Vector2 _spawnPoint);

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

	const float getX() const;

	const float getY() const;

	/// Handles collision with ALL tiles the player is colliding with
	void handleTileCollision(std::vector<Rectangle> &others);

private:
	float _dx, _dy;
	Direction _facing;
	bool _grounded;
};