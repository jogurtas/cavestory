#include "Player.h"
#include "Graphics.h"

namespace player_constants {
	constexpr float WALK_SPEED = 0.2f;
	constexpr float JUMP_SPEED = 0.7f;

	constexpr float GRAVITY = 0.002f;
	constexpr float GRAVITY_CAP = 0.8f;
}

Player::Player() {}

Player::Player(Graphics &gfx, Vector2 spawnPoint) :
	AnimatedSprite(gfx, "../assets/MyChar.png", 0, 0, 16, 16, spawnPoint.x, spawnPoint.y, 100),
	_dx(0),
	_dy(0),
	_facing(RIGHT),
	_lookingUp(false),
	_lookingDown(false),
	_maxHealth(3),
	_currentHealth(3),
	_grounded(false) {
	setupAnimations();
}

void Player::draw(Graphics &gfx) {
	AnimatedSprite::draw(gfx, (int)x, (int)y);
}

void Player::update(float deltaTime) {
	if (_dy <= player_constants::GRAVITY_CAP) {
		_dy += player_constants::GRAVITY * deltaTime;
	}

	// Move by dx
	x += _dx * deltaTime;
	// Move by dy
	y += _dy * deltaTime;

	AnimatedSprite::update(deltaTime);
}

void Player::animationDone(std::string currentAnimation) {
}

void Player::setupAnimations() {
	this->addAnimation(1, 0, 0, "IdleLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 0, 16, "IdleRight", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 0, 0, "RunLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 0, 16, "RunRight", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 3, 0, "IdleLeftUp", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 3, 16, "IdleRightUp", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 3, 0, "RunLeftUp", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 3, 16, "RunRightUp", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 6, 0, "LookDownLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 6, 16, "LookDownRight", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 7, 0, "LookBackwardsLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 7, 16, "LookBackwardsRight", 16, 16, Vector2(0, 0));
}

void Player::moveLeft() {
	if (_lookingDown && _grounded)
		return;
	
	_dx = -player_constants::WALK_SPEED;
	if (!_lookingUp) {
		playAnimation("RunLeft");
	}
	_facing = LEFT;
}

void Player::moveRight() {
	if (_lookingDown && _grounded)
		return;

	_dx = player_constants::WALK_SPEED;
	if (!_lookingUp) {
		playAnimation("RunRight");
	}
	_facing = RIGHT;
}

void Player::stopMoving() {
	_dx = 0.0f;

	if (!_lookingUp && !_lookingDown) {
		playAnimation(_facing == RIGHT ? "IdleRight" : "IdleLeft");
	}
}

void Player::lookUp() {
	_lookingUp = true;
	if (_dx == 0) {
		playAnimation(_facing == RIGHT ? "IdleRightUp" : "IdleLeftUp");
	}
	else {
		playAnimation(_facing == RIGHT ? "RunRightUp" : "RunLeftUp");
	}
}

void Player::stopLookingUp() {
	_lookingUp = false;
}

void Player::lookDown() {
	_lookingDown = true;
	if (_grounded) {
		// We need to interact (turn backwards)
		playAnimation(_facing == RIGHT ? "LookBackwardsRight" : "LookBackwardsLeft");
	}
	else {
		playAnimation(_facing == RIGHT ? "LookingDownRight" : "LookingDownLeft");
	}
}

void Player::stopLookingDown() {
	_lookingDown = false;
}

void Player::jump() {
	if (_grounded && !_lookingDown) {
		_dy = 0;
		_dy -= player_constants::JUMP_SPEED;
		_grounded = false;
	}
}

const float Player::getX() const {
	return x;
}

const float Player::getY() const {
	return y;
}

void Player::handleTileCollision(std::vector<Rectangle> &others) {
	// Figure out what side collision happened on and move the player accordingly
	for (Uint32 i = 0; i < others.size(); i++) {
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
			case sides::TOP:
				_dy = 0.0f;
				y = (float)(others.at(i).getBottom() + 1);
				if (_grounded) {
					_dx = 0;
					x -= _facing == RIGHT ? 0.5f : -0.5f;
				}
				break;
			case sides::BOTTOM:
				y = (float)(others.at(i).getTop() - _boundingBox.getHeight() - 1);
				_dy = 0.0f;
				_grounded = true;
				break;
			case sides::LEFT:
				x = (float)(others.at(i).getRight() + 1);
				break;
			case sides::RIGHT:
				x = (float)(others.at(i).getLeft() - _boundingBox.getWidth() - 1);
				break;
			case sides::NONE:
				break;
			}
		}
	}
}

void Player::handleSlopeCollision(std::vector<Slope> &others) {
	for (size_t i = 0; i < others.size(); i++) {
		// Calculate where on the slope the player's bottom center is touching
		// and use y=mx+b to figure  out the y possition to place him at.
		// First calculate b (slope intercept) using of the points (b = y - mx)
		int b = others.at(i).getP1().y - (others.at(i).getSlope() * fabs(others.at(i).getP1().x));

		// Get player's center x
		int centerX = _boundingBox.getCenterX();

		// Pass X into the equation y = mx + b to get the new y possition
		int newY = (others.at(i).getSlope() * centerX) + b - 8;

		// Re-possition the player to the correct y
		if (_grounded) {
			y = newY - _boundingBox.getHeight();
			_grounded = true;
		}
	}
}

void Player::handleDoorCollision(std::vector<Door> &others, Level &level, Graphics &gfx) {
	// Check if player is grounded and holding the down arrow
	// If so, go through the door
	for (size_t i = 0; i < others.size(); i++) {
		if (_grounded && _lookingDown) {
			level = Level(others.at(i).getDestination(), gfx);
			x = level.getPlayerSpawnPoint().x;
			y = level.getPlayerSpawnPoint().y;
		}
	}
}
