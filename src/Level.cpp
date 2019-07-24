#include <SDL.h>
#include "Level.h"
#include "Graphics.h"
#include "Globals.h"

Level::Level() {}

Level::Level(std::string mapName, Vector2 spawnPoint, Graphics &gfx) :
	_mapName(mapName),
	_spawnPoint(spawnPoint),
	_size(Vector2(0, 0)) {
	loadMap(mapName, gfx);
}

Level::~Level() {}

void Level::update(int deltaTime) {
}

void Level::draw(Graphics &gfx) {
	SDL_Rect srcRect = { 0, 0, 64, 64 };
	SDL_Rect dstRect;

	for (int x = 0; x < _size.x / 64; x++) {
		for (int y = 0; y < _size.y / 64; y++) {
			dstRect.x = x * 64 * globals::SPRITE_SCALE;
			dstRect.y = y * 64 * globals::SPRITE_SCALE;
			dstRect.w = 64 * globals::SPRITE_SCALE;
			dstRect.h = 64 * globals::SPRITE_SCALE;
			gfx.blitSurface(_backgroundTexture, &srcRect, &dstRect);
		}
	}
}

void Level::loadMap(std::string mapName, Graphics &gfx) {
	_backgroundTexture = SDL_CreateTextureFromSurface(gfx.getRenderer(), gfx.loadImage("../assets/bkBlue.png"));
	_size = Vector2(1280, 960);
}
