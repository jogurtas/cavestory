#include <sstream>
#include <algorithm>
#include <cmath>
#include <SDL.h>

#include "tinyxml2.h"
#include "Level.h"
#include "Graphics.h"
#include "Globals.h"

using namespace tinyxml2;

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
	for (Uint32 i = 0; i < _tileList.size(); i++) {
		_tileList.at(i).draw(gfx);
	}
}

void Level::loadMap(std::string mapName, Graphics &gfx) {
	// Parse the .tmx file
	XMLDocument doc;
	std::stringstream ss;
	ss << "../assets/" << mapName << ".tmx";
	doc.LoadFile(ss.str().c_str());

	XMLElement *mapNode = doc.FirstChildElement("map");

	// Get width and height of the whole map and store it in _size
	int width, height;
	mapNode->QueryIntAttribute("width", &width);
	mapNode->QueryIntAttribute("height", &height);
	_size = Vector2(width, height);

	// Get the width and the height of the tiles and store it in _tileSize
	int tileWidth, tileHeight;
	mapNode->QueryIntAttribute("tilewidth", &tileWidth);
	mapNode->QueryIntAttribute("tileheight", &tileHeight);
	_tileSize = Vector2(tileWidth, tileHeight);

	// Load the tileset image
	XMLElement *pTileset = mapNode->FirstChildElement("tileset");
	if (pTileset != nullptr) {
		while (pTileset) {
			int firstgid;
			const char *src = pTileset->FirstChildElement("image")->Attribute("source");
			std::stringstream ss;
			ss << "../assets/" << src;
			pTileset->QueryIntAttribute("firstgid", &firstgid);
			SDL_Texture *tex = SDL_CreateTextureFromSurface(gfx.getRenderer(), gfx.loadImage(ss.str()));
			_tilesets.push_back(Tileset(tex, firstgid));

			pTileset = pTileset->NextSiblingElement("tileset");
		}
	}

	// Loading the layers
	XMLElement *pLayer = mapNode->FirstChildElement("layer");
	if (pLayer != nullptr) {
		while (pLayer) {
			// Loading the data element
			XMLElement *pData = pLayer->FirstChildElement("data");
			if (pData != nullptr) {
				while (pData) {
					// Loading the tile element
					XMLElement *pTile = pData->FirstChildElement("tile");
					if (pTile != nullptr) {
						int tileCount = 0;
						while (pTile) {
							// Build each individual tile here
							// if gid 0, no tile should be drawn. 
							if (pTile->IntAttribute("gid") == 0) {
								tileCount++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else {
									break;
								}
							}

							// Get the tilset for this specific gid
							int gid = pTile->IntAttribute("gid");
							Tileset tls;
							for (Uint32 i = 0; i < _tilesets.size(); i++) {
								if (_tilesets[i].firstGid <= gid) {
									// This is the tilset we want
									tls = _tilesets.at(i);
									break;
								}
							}

							if (tls.firstGid == -1) {
								// No tilset was found for this gid
								tileCount++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else {
									break;
								}
							}

							// Get the position of the tile in the level
							int xx = 0;
							int yy = 0;
							xx = tileCount % width;
							xx *= tileWidth;
							yy += tileHeight * (tileCount / width);
							Vector2 finalTilePostition = Vector2(xx, yy);

							// Calculate the position of the tile in the tileset
							int tilesetWidth, tilesetHeight;
							SDL_QueryTexture(tls.texture, nullptr, nullptr, &tilesetWidth, &tilesetHeight);
							int tsxx = gid % (tilesetWidth / tileWidth) - 1;
							tsxx *= tileWidth;
							int tsyy = 0;
							int amt = (gid / (tilesetWidth / tileWidth));
							tsyy = tileHeight * amt;
							Vector2 finalTilsetPosition = Vector2(tsxx, tsyy);

							// Build the actual tile and add it to the level's tile list
							Tile tile(tls.texture, Vector2(tileWidth, tileHeight), finalTilsetPosition, finalTilePostition);
							_tileList.push_back(tile);
							tileCount++;

							pTile = pTile->NextSiblingElement("tile");
						}
					}

					pData = pData->NextSiblingElement("data");
				}
			}

			pLayer = pLayer->NextSiblingElement("layer");
		}
	}
}
