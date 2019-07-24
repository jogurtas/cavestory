#pragma once

#include <string>
#include "Globals.h"

class Graphics;
struct SDL_Texture;

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
	SDL_Texture *_backgroundTexture;

	void loadMap(std::string mapName, Graphics &gfx);
};
