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

	void setSourceRectX(int value);

	void setSourceRectY(int value);

	void setSourceRectWidth(int value);

	void setSourceRectHeight(int value);

	const Rectangle getBoundingBox() const;

	/// Determine which side the collision happend
	const sides::Side getCollisionSide(Rectangle &other) const;

	const inline float getX() const { return x; }

	const inline float getY() const { return y; }

protected:
    SDL_Rect srcRect;
    SDL_Texture *spriteSheet;

	Rectangle _boundingBox;

	float x;
	float y;
private:
};
