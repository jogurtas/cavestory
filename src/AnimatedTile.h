#pragma once

#include <vector>
#include <SDL.h>
#include "Globals.h"
#include "Tile.h"

class Graphics;

class AnimatedTile : public Tile {
public:
	AnimatedTile(std::vector<Vector2> tilsetPositions, int duration, SDL_Texture *tilset, Vector2 size, Vector2 position);
	
	void update(float deltaTime);

	void draw(Graphics &gfx);

protected:
	int _amountOfTime = 0;
	bool _notDone = false;
private:
	std::vector<Vector2> _tilesetPositions;
	int _tileToDraw;
	int _duration;
};


struct AnimatedTileInfo {
	int TilesetsFirstGid;
	int StartTileId;
	std::vector<int> TileIds;
	int Duration;
};