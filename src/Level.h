#pragma once

#include <string>
#include <vector>
#include "Globals.h"
#include "Tile.h"

class Graphics;
struct SDL_Texture;
struct SDL_Rect;
struct Tileset;

class Level {
public:
	Level();
	
	Level(std::string mapName, Vector2 spawnPoint, Graphics &gfx);
	
	~Level();

	void update(int deltaTime);

	void draw(Graphics &gfx);

private:
	std::string _mapName;
	Vector2 _spawnPoint;
	Vector2 _size;
	Vector2 _tileSize;
	SDL_Texture *_backgroundTexture;

	std::vector<Tile> _tileList;
	std::vector<Tileset> _tilesets;

	void loadMap(std::string mapName, Graphics &gfx);
};

// Tileset struct
struct Tileset {
	SDL_Texture *texture;
	int firstGid;

	Tileset() {
		firstGid = -1;
	}

	Tileset(SDL_Texture *texture, int firstGid) : texture(texture), firstGid(firstGid) {}
};
