#pragma once

#include "AnimatedSprite.h"
#include "Globals.h"
#include "Slope.h"
#include "Door.h"
#include "Level.h"

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

	void lookUp();

	void stopLookingUp();

	void lookDown();

	void stopLookingDown();

	/// Starts jumping
	void jump();

	const float getX() const;

	const float getY() const;

	/// Handles collision with ALL tiles the player is colliding with
	void handleTileCollision(std::vector<Rectangle> &others);

	void handleSlopeCollision(std::vector<Slope> &others);

	void handleDoorCollision(std::vector<Door> &others, Level &level, Graphics &gfx);

	const inline int getMaxHealth() const { return _maxHealth; }

	const inline int getCurrentHealth() const { return _currentHealth; }

private:
	float _dx, _dy;
	Direction _facing;
	bool _grounded;
	bool _lookingUp;
	bool _lookingDown;

	int _maxHealth;
	int _currentHealth;
};