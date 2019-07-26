#pragma once

#include <SDL.h>
#include <string>
#include "Rectangle.h"
#include "Globals.h"

class Graphics;

class Sprite {
public:
    Sprite();

    Sprite(Graphics &graphics, const std::string &filePath,
           int srcX, int srcY, int width, int height,
           float posX, float posY);

    virtual ~Sprite();

    virtual void update();

    void draw(Graphics &graphics, int xPos, int yPos);

	const Rectangle getBoundingBox() const;

	/// Determine which side the collision happend
	const sides::Side getCollisionSide(Rectangle &other) const;

protected:
    SDL_Rect srcRect;
    SDL_Texture *spriteSheet;

	Rectangle _boundingBox;

	float x;
	float y;
private:
};
