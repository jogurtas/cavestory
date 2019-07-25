#include <SDL.h>
#include "Tile.h"
#include "Graphics.h"

Tile::Tile() {}

Tile::Tile(SDL_Texture *tileset, Vector2 size, Vector2 tilesetPos, Vector2 pos) :
	_tileset(tileset),
	_size(size),
	_tilesetPos(tilesetPos),
	_pos(Vector2(pos.x *(int)globals::SPRITE_SCALE, pos.y *(int)globals::SPRITE_SCALE)) {
}

void Tile::update(float deltaTime) {}

void Tile::draw(Graphics &gfx) {
	SDL_Rect dstRect = { _pos.x, _pos.y,
		_size.x * (int)globals::SPRITE_SCALE, _size.y * (int)globals::SPRITE_SCALE };
	SDL_Rect srcRect = { _tilesetPos.x, _tilesetPos.y, _size.x, _size.y };

	gfx.blitSurface(_tileset, &srcRect, &dstRect);
}
