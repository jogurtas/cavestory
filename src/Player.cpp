#include "Player.h"
#include "Graphics.h"

namespace player_constants {
	constexpr float WALK_SPEED = 0.2f;
	constexpr float GRAVITY = 0.002f;
	constexpr float GRAVITY_CAP = 0.8f;
}

Player::Player() {}

Player::Player(Graphics &gfx, Vector2 spawnPoint) :
	AnimatedSprite(gfx, "../assets/MyChar.png", 0, 0, 16, 16, spawnPoint.x, spawnPoint.y, 100), 
	_dx(0), 
	_dy(0),
	_facing(RIGHT),
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
	addAnimation(1, 0, 0, "IdleLeft", 16, 16, Vector2(0, 0));
	addAnimation(1, 0, 16, "IdleRight", 16, 16, Vector2(0, 0));
	addAnimation(3, 0, 0, "RunLeft", 16, 16, Vector2(0, 0));
	addAnimation(3, 0, 16, "RunRight", 16, 16, Vector2(0, 0));
}

void Player::moveLeft() {
	_dx = -player_constants::WALK_SPEED;
	playAnimation("RunLeft");
	_facing = LEFT;
}

void Player::moveRight() {
	_dx = player_constants::WALK_SPEED;
	playAnimation("RunRight");
	_facing = RIGHT;
}

void Player::stopMoving() {
	_dx = 0.0f;
	playAnimation(_facing == RIGHT ? "IdleRight" : "IdleLeft");
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
				y = (float)(others.at(i).getBottom() + 1);
				_dy = 0.0f;
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
