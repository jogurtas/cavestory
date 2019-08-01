#pragma once

#include <string>
#include <vector>
#include "Globals.h"
#include "Tile.h"
#include "Rectangle.h"
#include "Slope.h"
#include "AnimatedTile.h"
#include "Door.h"

class Graphics;
class Enemy;
class Player;
struct SDL_Texture;
struct SDL_Rect;
struct Tileset;

class Level {
public:
	Level();
	
	Level(std::string mapName, Graphics &gfx);
	
	~Level();

	void update(float deltaTime, Player &player);

	void draw(Graphics &gfx);

	std::vector<Rectangle> checkTileCollisions(const Rectangle &other);

	std::vector<Slope> checkSlopeCollisions(const Rectangle &other);

	std::vector<Door> checkDoorCollision(const Rectangle &other);

	std::vector<Enemy *> checkEnemyCollision(const Rectangle &other);

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

	std::vector<Door> _doorList;
	std::vector<Enemy *> _enemies;

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
