#pragma once

#include <string>
#include "Rectangle.h"
#include "Globals.h"

class Door : public Rectangle {
public:
	Door() {};

	Door(Rectangle r, std::string destination) :
		Rectangle(r.getLeft() * (int)globals::SPRITE_SCALE, r.getTop() * (int)globals::SPRITE_SCALE,
			r.getWidth() * (int)globals::SPRITE_SCALE, r.getHeight() * (int)globals::SPRITE_SCALE),
		_destination(destination) {
	}

	const inline Rectangle getRectangle() const { return getRect(); }

	const inline std::string getDestination() const { return _destination; }

private:
	std::string _destination;
};
