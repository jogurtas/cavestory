#include <sstream>
#include <algorithm>
#include <cmath>
#include <SDL.h>

#include "tinyxml2.h"
#include "Level.h"
#include "Graphics.h"
#include "Globals.h"
#include "Utils.h"

using namespace tinyxml2;

Level::Level() {}

Level::Level(std::string mapName, Graphics &gfx) :
	_mapName(mapName),
	_size(Vector2(0, 0)) {
	loadMap(mapName, gfx);
}

Level::~Level() {}

void Level::update(float deltaTime) {
	for (size_t i = 0; i < _animatedTileList.size(); i++) {
		_animatedTileList.at(i).update(deltaTime);
	}
}

void Level::draw(Graphics &gfx) {
	for (Uint32 i = 0; i < _tileList.size(); i++) {
		_tileList.at(i).draw(gfx);
	}
	for (size_t i = 0; i < _animatedTileList.size(); i++) {
		_animatedTileList.at(i).draw(gfx);
	}
}

std::vector<Rectangle> Level::checkTileCollisions(const Rectangle &other) {
	std::vector<Rectangle> others;
	for (Uint32 i = 0; i < _collisionRects.size(); i++) {
		if (_collisionRects.at(i).collidesWith(other)) {
			others.push_back(_collisionRects.at(i));
		}
	}

	return others;
}

std::vector<Slope> Level::checkSlopeCollisions(const Rectangle &other) {
	std::vector<Slope> others;
	for (size_t i = 0; i < _slopes.size(); i++) {
		if (_slopes.at(i).collidesWith(other)) {
			others.push_back(_slopes.at(i));
		}
	}

	return others;
}

std::vector<Door> Level::checkDoorCollision(const Rectangle &other) {
	std::vector<Door> others;
	for (size_t i = 0; i < _doorList.size(); i++) {
		if (_doorList.at(i).collidesWith(other)) {
			others.push_back(_doorList.at(i));
		}
	}

	return others;
}

const Vector2 Level::getPlayerSpawnPoint() const {
	return _spawnPoint;
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


			// Get all of the animations for that tileset
			XMLElement *pTileA = pTileset->FirstChildElement("tile");
			if (pTileA != NULL) {
				while (pTileA) {
					AnimatedTileInfo ati;
					ati.StartTileId = pTileA->IntAttribute("id") + firstgid;
					ati.TilesetsFirstGid = firstgid;
					XMLElement *pAnimation = pTileA->FirstChildElement("animation");
					if (pAnimation != NULL) {
						while (pAnimation) {
							XMLElement *pFrame = pAnimation->FirstChildElement("frame");
							if (pFrame != NULL) {
								while (pFrame) {
									ati.TileIds.push_back(pFrame->IntAttribute("tileid") + firstgid);
									ati.Duration = pFrame->IntAttribute("duration");
									pFrame = pFrame->NextSiblingElement("frame");
								}
							}
							pAnimation = pAnimation->NextSiblingElement("animation");
						}
					}
					this->_animatedTileInfo.push_back(ati);
					pTileA = pTileA->NextSiblingElement("tile");
				}
			}
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
							int closest = 0;
							for (Uint32 i = 0; i < _tilesets.size(); i++) {
								if (_tilesets[i].firstGid <= gid) {
									if (_tilesets[i].firstGid > closest) {
										closest = _tilesets[i].firstGid;
										tls = _tilesets.at(i);
									}
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
							Vector2 finalTilePosition = Vector2(xx, yy);

							//Calculate the position of the tile in the tileset
							Vector2 finalTilesetPosition = getTilesetPosition(tls, gid, tileWidth, tileHeight);

							//Build the actual tile and add it to the level's tile list
							bool isAnimatedTile = false;
							AnimatedTileInfo ati;
							for (size_t i = 0; i < _animatedTileInfo.size(); i++) {
								if (_animatedTileInfo.at(i).StartTileId == gid) {
									ati = _animatedTileInfo.at(i);
									isAnimatedTile = true;
									break;
								}
							}
							if (isAnimatedTile) {
								std::vector<Vector2> tilesetPositions;
								for (size_t i = 0; i < ati.TileIds.size(); i++) {
									tilesetPositions.push_back(getTilesetPosition(tls, ati.TileIds.at(i),
										tileWidth, tileHeight));
								}
								AnimatedTile tile(tilesetPositions, ati.Duration,
									tls.texture, Vector2(tileWidth, tileHeight), finalTilePosition);
								_animatedTileList.push_back(tile);
							}
							else {
								Tile tile(tls.texture, Vector2(tileWidth, tileHeight),
									finalTilesetPosition, finalTilePosition);
								_tileList.push_back(tile);
							}
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

	// Parse out the collisions
	XMLElement *pObjectGroup = mapNode->FirstChildElement("objectgroup");
	if (pObjectGroup != nullptr) {
		while (pObjectGroup) {
			const char *name = pObjectGroup->Attribute("name");
			std::stringstream ss;
			ss << name;
			if (ss.str() == "collisions") {
				XMLElement *pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != nullptr) {
					while (pObject) {
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");
						_collisionRects.push_back(Rectangle(
							(int)(std::ceil(x) * globals::SPRITE_SCALE),
							(int)(std::ceil(y) * globals::SPRITE_SCALE),
							(int)(std::ceil(width) * globals::SPRITE_SCALE),
							(int)(std::ceil(height) * globals::SPRITE_SCALE)
						));

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			// Other objectgroups go here with else uf (ss.str() == "groupname")
			else if (ss.str() == "spawn points") {
				XMLElement *pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != nullptr) {
					while (pObject) {
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						const char *name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;
						if (ss.str() == "player") {
							_spawnPoint = Vector2((int)(std::ceil(x) * globals::SPRITE_SCALE),
								(int)(std::ceil(y) * globals::SPRITE_SCALE));
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "slopes") {
				XMLElement *pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						std::vector<Vector2> points;
						Vector2 p1;
						p1 = Vector2((int)std::ceil(pObject->FloatAttribute("x")), (int)std::ceil(pObject->FloatAttribute("y")));

						XMLElement *pPolyline = pObject->FirstChildElement("polyline");
						if (pPolyline != NULL) {
							std::vector<std::string> pairs;
							const char *pointString = pPolyline->Attribute("points");

							std::stringstream ss;
							ss << pointString;
							Utils::split(ss.str(), pairs, ' ');
							//Now we have each of the pairs. Loop through the list of pairs
							//and split them into Vector2s and then store them in our points vector
							for (size_t i = 0; i < pairs.size(); i++) {
								std::vector<std::string> ps;
								Utils::split(pairs.at(i), ps, ',');
								points.push_back(Vector2(std::stoi(ps.at(0)), std::stoi(ps.at(1))));
							}
						}

						for (size_t i = 0; i < points.size(); i += 2) {
							this->_slopes.push_back(Slope(
								Vector2((p1.x + points.at(i < 2 ? i : i - 1).x) * (int)globals::SPRITE_SCALE,
								(p1.y + points.at(i < 2 ? i : i - 1).y) * (int)globals::SPRITE_SCALE),
								Vector2((p1.x + points.at(i < 2 ? i + 1 : i).x) * (int)globals::SPRITE_SCALE,
								(p1.y + points.at(i < 2 ? i + 1 : i).y) * (int)globals::SPRITE_SCALE)
							));
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "doors") {
				XMLElement *pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != nullptr) {
					while (pObject) {
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						float w = pObject->FloatAttribute("width");
						float h = pObject->FloatAttribute("height");
						Rectangle rect = Rectangle(x, y, w, h);

						XMLElement *pProperties = pObject->FirstChildElement("properties");
						if (pProperties != nullptr) {
							while (pProperties) {
								XMLElement *pProperty = pProperties->FirstChildElement("property");
								if (pProperty != nullptr) {
									while (pProperty) {
										const char *name = pProperty->Attribute("name");
										std::stringstream ss;
										ss << name;
										if (ss.str() == "destination") {
											const char *value = pProperty->Attribute("value");
											std::stringstream ss2;
											ss2 << value;
											Door door = Door(rect, ss2.str());
											this->_doorList.push_back(door);
										}
										pProperty = pProperty->NextSiblingElement("property");
									}
								}
								pProperties = pProperties->NextSiblingElement("properties");
							}
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}

			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
		}
	}
}

Vector2 Level::getTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight) {
	int tilesetWidth, tilesetHeight;
	SDL_QueryTexture(tls.texture, nullptr, nullptr, &tilesetWidth, &tilesetHeight);
	int tsxx = (gid - 1) % (tilesetWidth / tileWidth);
	tsxx *= tileWidth;
	int tsyy = 0;
	int amt = ((gid - tls.firstGid) / (tilesetWidth / tileWidth));
	tsyy = tileHeight * amt;
	return Vector2(tsxx, tsyy);
}
