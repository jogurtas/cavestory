#include "Player.h"
#include "Graphics.h"

namespace player_constants {
	constexpr float WALK_SPEED = 0.2f;
}

Player::Player() {}

Player::Player(Graphics &gfx, float x, float y) :
	AnimatedSprite(gfx, "../assets/MyChar.png", 0, 0, 16, 16, x, y, 100), 
	_dx(0), 
	_dy(0),
	_facing(RIGHT) {
	setupAnimations();
}

void Player::draw(Graphics &gfx) {
	AnimatedSprite::draw(gfx, x, y);
}

void Player::update(float deltaTime) {
	x += _dx * deltaTime;
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
