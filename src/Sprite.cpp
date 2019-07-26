#include <iostream>
#include "Sprite.h"
#include "Graphics.h"
#include "Globals.h"

Sprite::Sprite() {}

Sprite::Sprite(Graphics &graphics, const std::string &filePath,
	int srcX, int srcY, int width, int height,
	float posX, float posY) :
	x(posX),
	y(posY) {
	srcRect.x = srcX;
	srcRect.y = srcY;
	srcRect.w = width;
	srcRect.h = height;

	spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath));
	if (spriteSheet == nullptr) {
		std::cout << "Error: Unable to load image\n";
	}

	_boundingBox = Rectangle(x, y, width * (int)globals::SPRITE_SCALE, height * (int)globals::SPRITE_SCALE);
}

Sprite::~Sprite() {}

void Sprite::update() {
	_boundingBox = Rectangle(x, y,
		srcRect.w * (int)globals::SPRITE_SCALE, srcRect.h * (int)globals::SPRITE_SCALE);
}

void Sprite::draw(Graphics &graphics, int xPos, int yPos) {
	SDL_Rect dstRect = { xPos, yPos,
						static_cast<int>(srcRect.w * globals::SPRITE_SCALE),
						static_cast<int>(srcRect.h * globals::SPRITE_SCALE) };
	graphics.blitSurface(spriteSheet, &srcRect, &dstRect);
}

const Rectangle Sprite::getBoundingBox() const {
	return _boundingBox;
}

const sides::Side Sprite::getCollisionSide(Rectangle &other) const {
	int amtRight = getBoundingBox().getRight() - other.getLeft();
	int amtLeft = other.getRight() - getBoundingBox().getLeft();
	int amtTop = other.getBottom() - getBoundingBox().getTop();
	int amtBottom = getBoundingBox().getBottom() - other.getTop();

	int vals[4] = { abs(amtRight), abs(amtLeft), abs(amtTop), abs(amtBottom) };
	int lowest = vals[0];
	for (int i = 0; i < 4; i++) {
		if (vals[i] < lowest) {
			lowest = vals[i];
		}
	}

	return
		lowest == abs(amtRight) ? sides::RIGHT :
		lowest == abs(amtLeft) ? sides::LEFT :
		lowest == abs(amtTop) ? sides::TOP :
		lowest == abs(amtBottom) ? sides::BOTTOM :
		sides::NONE;
}
