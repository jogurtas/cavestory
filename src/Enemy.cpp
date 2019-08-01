#include "Enemy.h"

// Enemy class
Enemy::Enemy() {}

Enemy::Enemy(Graphics &gfx, std::string filePath, int srcX, int srcY, 
	int width, int height, Vector2 spawnPoint, int timeToUpdate) : 
	AnimatedSprite(gfx, filePath, srcX, srcY, width, height, spawnPoint.x, spawnPoint.y, timeToUpdate),
	_direction(LEFT),
	_maxHealth(0),
	_currentHealth(0) {
}

void Enemy::update(int deltaTime, Player &player) {
	AnimatedSprite::update(deltaTime);
}

void Enemy::draw(Graphics &gfx) {
	AnimatedSprite::draw(gfx, x, y);
}

// Bat class
Bat::Bat() {}

Bat::Bat(Graphics &gfx, Vector2 spawnPoint) :
	Enemy(gfx, "../assets/NpcCemet.png", 32, 32, 16, 16, spawnPoint, 140),
	_startingX(spawnPoint.x), 
	_startingY(spawnPoint.y),
	_shouldMoveUp(false) {
	setupAnimations();
	playAnimation("FlyLeft");
}

void Bat::update(int deltaTime, Player &player) {
	_direction = player.getX() > x ? RIGHT : LEFT;
	playAnimation(_direction == RIGHT ? "FlyRight" : "FlyLeft");

	// Move up or down
	y += _shouldMoveUp ? -0.05f : 0.05f;
	if (y > (_startingY + 20) || y < _startingY - 20) {
		_shouldMoveUp = !_shouldMoveUp;
	}

	Enemy::update(deltaTime, player);
}

void Bat::draw(Graphics &gfx) {
	Enemy::draw(gfx);
}

void Bat::animationDone(std::string currentAnimation) {
}

void Bat::setupAnimations() {
	addAnimation(3, 2, 32, "FlyLeft", 16, 16, Vector2(0, 0));
	addAnimation(3, 2, 48, "FlyRight", 16, 16, Vector2(0, 0));
}

void Bat::touchPlayer(Player *player) {
	player->gainHealth(-1);
}
