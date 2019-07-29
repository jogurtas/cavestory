#include "AnimatedTile.h"
#include "Graphics.h"

AnimatedTile::AnimatedTile(std::vector<Vector2> tilsetPositions, int duration, SDL_Texture *tilset, Vector2 size, Vector2 position) :
	Tile(tilset, size, tilsetPositions.at(0), position),
	_tilesetPositions(tilsetPositions),
	_duration(duration),
	_tileToDraw(0) {

}

void AnimatedTile::update(float deltaTime) {
	if (_amountOfTime <= 0) {
		if (_tileToDraw == _tilesetPositions.size() - 1) {
			_tileToDraw = 0;
		}
		else {
			_tileToDraw++;
		}
		_amountOfTime = _duration;
	}
	else {
		_amountOfTime -= (int)deltaTime;
	}

	Tile::update(deltaTime);
}

void AnimatedTile::draw(Graphics &gfx) {
	SDL_Rect dstRect = { _pos.x, _pos.y, _size.x * (int)globals::SPRITE_SCALE, _size.y * (int)globals::SPRITE_SCALE };
	SDL_Rect srcRect = { _tilesetPositions.at(_tileToDraw).x, _tilesetPositions.at(_tileToDraw).y,
	_size.x, _size.y };

	gfx.blitSurface(_tileset, &srcRect, &dstRect);
}
