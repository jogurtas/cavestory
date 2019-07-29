#pragma once
#include "Globals.h"
#include "Rectangle.h"

class Slope {
public:
	Slope() {}
	Slope(Vector2 p1, Vector2 p2) : _p1(p1), _p2(p2) {
		if (_p2.x - p1.x != 0) {
			_slope = (float)((fabs(_p2.y) - fabs(_p1.y)) / (fabs(_p2.x) - fabs(_p1.x)));
		}
	}

	const inline float getSlope() const {
		return _slope;
	}

	const bool collidesWith(const Rectangle &other) const {
		return
			(other.getRight() >= this->_p2.x &&
				other.getLeft() <= this->_p1.x &&
				other.getTop() <= this->_p2.y &&
				other.getBottom() >= this->_p1.y) ||
				(other.getRight() >= this->_p1.x &&
					other.getLeft() <= this->_p2.x &&
					other.getTop() <= this->_p1.y &&
					other.getBottom() >= this->_p2.y) ||
					(other.getLeft() <= this->_p1.x &&
						other.getRight() >= this->_p2.x &&
						other.getTop() <= this->_p1.y &&
						other.getBottom() >= this->_p2.y) ||
						(other.getLeft() <= this->_p2.x &&
							other.getRight() >= this->_p1.x &&
							other.getTop() <= this->_p2.y &&
							other.getBottom() >= this->_p1.y);
	}

	const inline Vector2 getP1() const { return _p1; }

	const inline Vector2 getP2() const { return _p2; }

private:
	Vector2 _p1, _p2;
	float _slope;
};
