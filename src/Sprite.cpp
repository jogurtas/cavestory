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
}

Sprite::~Sprite() {}

void Sprite::update() {

}

void Sprite::draw(Graphics &graphics, int xPos, int yPos) {
    SDL_Rect dstRect = {xPos, yPos,
                        static_cast<int>(srcRect.w * globals::SPRITE_SCALE),
                        static_cast<int>(srcRect.h * globals::SPRITE_SCALE)};
    graphics.blitSurface(spriteSheet, &srcRect, &dstRect);
}
