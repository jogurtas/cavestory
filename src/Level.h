#pragma once

#include <string>
#include <vector>
#include "Globals.h"
#include "Tile.h"
#include "Rectangle.h"
#include "Slope.h"
#include "AnimatedTile.h"

class Graphics;
struct SDL_Texture;
struct SDL_Rect;
struct Tileset;

class Level {
public:
	Level();
	
	Level(std::string mapName, Vector2 spawnPoint, Graphics &gfx);
	
	~Level();

	void update(float deltaTime);

	void draw(Graphics &gfx);

	std::vector<Rectangle> checkTileCollisions(const Rectangle &other);

	std::vector<Slope> checkSlopeCollisions(const Rectangle &other);

	const Vector2 getPlayerSpawnPoint() const;

private:
	std::string _mapName;
	Vector2 _spawnPoint;
	Vector2 _size;
	Vector2 _tileSize;
	SDL_Texture *_backgroundTexture;

	std::vector<Tile> _tileList;
	std::vector<Tileset> _tilesets;
	std::vector<Rectangle> _collisionRects;
	std::vector<Slope> _slopes;
	std::vector<AnimatedTile> _animatedTileList;
	std::vector<AnimatedTileInfo> _animatedTileInfo;

	void loadMap(std::string mapName, Graphics &gfx);

	Vector2 getTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight);
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
