#include "level.h"
#include "graphics.h"
#include "globals.h"
#include "utils.h"
#include "player.h"
#include "enemy.h"
#include "bat.h"
#include "tinyxml2.h"

#include <SDL2/SDL.h>
#include "globals.h"

#include <sstream>
#include <algorithm>
#include <cmath>
#include "LevelPassage.h"
using namespace tinyxml2;
Level::Level() {}

Level::Level(std::string mapName, Graphics &graphics) : _mapName(mapName),
																												_size(Vector2(0, 0))
{
	this->loadMap(mapName, graphics);
}

Level::~Level()
{
}

void Level::loadMap(std::string mapName, Graphics &graphics)
{
	// Parse the .tmx file
	XMLDocument doc;
	std::stringstream ss;
	ss << "maps/" << mapName << ".tmx"; // Pass in Map 1, we get maps/Map 1.tmx
	doc.LoadFile(ss.str().c_str());

	XMLElement *mapNode = doc.FirstChildElement("map");

	// Get the width and the height of the whole map and store it in _size
	int width, height;
	mapNode->QueryIntAttribute("width", &width);
	mapNode->QueryIntAttribute("height", &height);
	this->_size = Vector2(width, height);
	// Get the width and the height of the tiles and store it in _tileSize
	int tileWidth, tileHeight;
	mapNode->QueryIntAttribute("tilewidth", &tileWidth);
	mapNode->QueryIntAttribute("tileheight", &tileHeight);
	this->_tileSize = Vector2(tileWidth, tileHeight);

	// Loading the tilesets
	XMLElement *pTileset = mapNode->FirstChildElement("tileset");
	if (pTileset != NULL)
	{
		while (pTileset)
		{
			int firstgid;
			const char *source = pTileset->FirstChildElement("image")->Attribute("source");
			std::string parsedString = this->parseString(source);
			pTileset->QueryIntAttribute("firstgid", &firstgid);
			SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(parsedString.erase(0, 3)));
			this->_tilesets.push_back(Tileset(tex, firstgid));

			// Get all of the animations for that tileset
			XMLElement *pTileA = pTileset->FirstChildElement("tile");
			if (pTileA != NULL)
			{
				while (pTileA)
				{
					AnimatedTileInfo ati;
					ati.StartTileId = pTileA->IntAttribute("id") + firstgid;
					ati.TilesetsFirstGid = firstgid;
					XMLElement *pAnimation = pTileA->FirstChildElement("animation");
					if (pAnimation != NULL)
					{
						while (pAnimation)
						{
							XMLElement *pFrame = pAnimation->FirstChildElement("frame");
							if (pFrame != NULL)
							{
								while (pFrame)
								{
									ati.TileIds.push_back(pFrame->IntAttribute("tileid") + firstgid);
									ati.Duration = pFrame->IntAttribute("duration");
									pFrame = pFrame->NextSiblingElement("frame");
								}
							}
							pAnimation = pAnimation->NextSiblingElement("animation");
						}
					}
					this->_animatedTileInfos.push_back(ati);
					pTileA = pTileA->NextSiblingElement("tile");
				}
			}
			pTileset = pTileset->NextSiblingElement("tileset");
		}
	}

	// Loading the layers
	XMLElement *pLayer = mapNode->FirstChildElement("layer");
	if (pLayer != NULL)
	{
		while (pLayer)
		{
			// Loading the data element
			XMLElement *pData = pLayer->FirstChildElement("data");
			if (pData != NULL)
			{
				while (pData)
				{
					// Loading the tile element
					XMLElement *pTile = pData->FirstChildElement("tile");
					if (pTile != NULL)
					{
						int tileCounter = 0;
						while (pTile)
						{
							// Build each individual tile here
							// If gid is 0, no tile should be drawn. Continue loop
							if (pTile->IntAttribute("gid") == 0)
							{
								tileCounter++;
								if (pTile->NextSiblingElement("tile"))
								{
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else
								{
									break;
								}
							}

							// Get the tileset for this specific gid
							int gid = pTile->IntAttribute("gid");
							Tileset tls;
							int closest = 0;
							for (int i = 0; i < this->_tilesets.size(); i++)
							{
								if (this->_tilesets[i].FirstGid <= gid)
								{
									if (this->_tilesets[i].FirstGid > closest)
									{
										closest = this->_tilesets[i].FirstGid;
										tls = this->_tilesets.at(i);
									}
								}
							}

							if (tls.FirstGid == -1)
							{
								// No tileset was found for this gid
								tileCounter++;
								if (pTile->NextSiblingElement("tile"))
								{
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else
								{
									break;
								}
							}

							// Get the position of the tile in the level
							int xx = 0;
							int yy = 0;
							xx = tileCounter % width;
							xx *= tileWidth;
							yy += tileHeight * (tileCounter / width);
							Vector2 finalTilePosition = Vector2(xx, yy);

							// Calculate the position of the tile in the tileset
							Vector2 finalTilesetPosition = this->getTilesetPosition(tls, gid, tileWidth, tileHeight);

							// Build the actual tile and add it to the level's tile list
							bool isAnimatedTile = false;
							AnimatedTileInfo ati;
							for (int i = 0; i < this->_animatedTileInfos.size(); i++)
							{
								if (this->_animatedTileInfos.at(i).StartTileId == gid)
								{
									ati = this->_animatedTileInfos.at(i);
									isAnimatedTile = true;
									break;
								}
							}
							if (isAnimatedTile == true)
							{
								std::vector<Vector2> tilesetPositions;
								for (int i = 0; i < ati.TileIds.size(); i++)
								{
									tilesetPositions.push_back(this->getTilesetPosition(tls, ati.TileIds.at(i),
																																			tileWidth, tileHeight));
								}
								AnimatedTile tile(tilesetPositions, ati.Duration,
																	tls.Texture, Vector2(tileWidth, tileHeight), finalTilePosition);
								this->_animatedTileList.push_back(tile);
							}
							else
							{
								Tile tile(tls.Texture, Vector2(tileWidth, tileHeight),
													finalTilesetPosition, finalTilePosition);
								this->_tileList.push_back(tile);
							}
							tileCounter++;
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
	if (pObjectGroup != NULL)
	{
		while (pObjectGroup)
		{
			const char *name = pObjectGroup->Attribute("name");
			std::string objectLayerName = this->parseString(name);
			if (objectLayerName == "collisions")
			{
				XMLElement *pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL)
				{
					while (pObject)
					{
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");
						this->_collisionRects.push_back(Rectangle(
								std::ceil(x) * globals::SPRITE_SCALE,
								std::ceil(y) * globals::SPRITE_SCALE,
								std::ceil(width) * globals::SPRITE_SCALE,
								std::ceil(height) * globals::SPRITE_SCALE));
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (objectLayerName == "slopes")
			{
				XMLElement *pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL)
				{
					while (pObject)
					{
						std::vector<Vector2> points;
						Vector2 p1;
						p1 = Vector2(std::ceil(pObject->FloatAttribute("x")), std::ceil(pObject->FloatAttribute("y")));

						XMLElement *pPolyline = pObject->FirstChildElement("polyline");
						if (pPolyline != NULL)
						{
							std::vector<std::string> pairs;
							const char *pPoints = pPolyline->Attribute("points");

							std::string pointsParsed = this->parseString(pPoints);
							Utils::split(pointsParsed, pairs, ' ');
							// Now we have each of the pairs. Loop through the list of pairs
							// and split them into Vector2s and then store them in our points vector
							for (int i = 0; i < pairs.size(); i++)
							{
								std::vector<std::string> ps;
								Utils::split(pairs.at(i), ps, ',');
								points.push_back(Vector2(std::stoi(ps.at(0)), std::stoi(ps.at(1))));
							}
						}

						for (int i = 0; i < points.size(); i += 2)
						{
							this->_slopes.push_back(Slope(
									Vector2((p1.x + points.at(i < 2 ? i : i - 1).x) * globals::SPRITE_SCALE,
													(p1.y + points.at(i < 2 ? i : i - 1).y) * globals::SPRITE_SCALE),
									Vector2((p1.x + points.at(i < 2 ? i + 1 : i).x) * globals::SPRITE_SCALE,
													(p1.y + points.at(i < 2 ? i + 1 : i).y) * globals::SPRITE_SCALE)));
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (objectLayerName == "spawn points")
			{
				XMLElement *pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL)
				{
					while (pObject)
					{
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						const char *name = pObject->Attribute("name");
						std::string parsedString = this->parseString(name);
						if (parsedString == "player")
						{
							this->_spawnPoint = Vector2(std::ceil(x) * globals::SPRITE_SCALE,
																					std::ceil(y) * globals::SPRITE_SCALE);
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (objectLayerName == "doors")
			{
				XMLElement *pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL)
				{
					while (pObject)
					{
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						float w = pObject->FloatAttribute("width");
						float h = pObject->FloatAttribute("height");
						Rectangle rect = Rectangle(x, y, w, h);
						Vector2 spawnPosition = {0, 0};
						std::string destination;
						XMLElement *pProperties = pObject->FirstChildElement("properties");
						if (pProperties != NULL)
						{
							while (pProperties)
							{
								XMLElement *pProperty = pProperties->FirstChildElement("property");
								if (pProperty != NULL)
								{
									while (pProperty)
									{
										const char *name = pProperty->Attribute("name");
										std::string propertyName = this->parseString(name);
										if (propertyName == "destination")
										{
											const char *value = pProperty->Attribute("value");
											destination = this->parseString(value);
										}
										if (propertyName == "spawn_position")
										{
											const char *value = pProperty->Attribute("value");
											spawnPosition = this->parsePosition(value);
										}
										pProperty = pProperty->NextSiblingElement("property");
									}
								}
								pProperties = pProperties->NextSiblingElement("properties");
							}
						}
						Door door = Door(rect, destination, spawnPosition);
						this->_doorList.push_back(door);
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (objectLayerName == "level_passages")
			{

				XMLElement *pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL)
				{
					while (pObject)
					{
						std::string destination;
						Vector2 spawnPosition = {0, 0};

						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						float w = pObject->FloatAttribute("width");
						float h = pObject->FloatAttribute("height");
						Rectangle rect = Rectangle(x, y, w, h);

						XMLElement *pProperties = pObject->FirstChildElement("properties");
						if (pProperties != NULL)
						{
							while (pProperties)
							{
								XMLElement *pProperty = pProperties->FirstChildElement("property");
								if (pProperty != NULL)
								{
									while (pProperty)
									{
										const char *name = pProperty->Attribute("name");
										std::string propertyName = this->parseString(name);
										if (propertyName == "destination")
										{
											const char *value = pProperty->Attribute("value");
											destination = this->parseString(value);
										}
										if (propertyName == "spawn_position")
										{
											const char *value = pProperty->Attribute("value");
											spawnPosition = this->parsePosition(value);
										}
										pProperty = pProperty->NextSiblingElement("property");
									}
								}
								pProperties = pProperties->NextSiblingElement("properties");
							}
						}

						LevelPassage levelPassage = LevelPassage(rect, destination, spawnPosition);
						this->_levelPassagesList.push_back(levelPassage);
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (objectLayerName == "enemies")
			{
				float x, y;
				XMLElement *pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL)
				{
					while (pObject)
					{
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						const char *name = pObject->Attribute("name");
						std::string enemyName = this->parseString(name);
						if (enemyName == "bat")
						{
							this->_enemies.push_back(new Bat(graphics, Vector2(std::floor(x) * globals::SPRITE_SCALE,
																																 std::floor(y) * globals::SPRITE_SCALE)));
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}

			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
		}
	}
}

void Level::update(int elapsedTime, Player &player)
{
	for (int i = 0; i < this->_animatedTileList.size(); i++)
	{
		this->_animatedTileList.at(i).update(elapsedTime);
	}
	for (int i = 0; i < this->_enemies.size(); i++)
	{
		this->_enemies.at(i)->update(elapsedTime, player);
	}
}

void Level::draw(Graphics &graphics, Player &player)
{

	for (int i = 0; i < this->_tileList.size(); i++)
	{

		this->_tileList.at(i).draw(graphics);
	}
	for (int i = 0; i < this->_animatedTileList.size(); i++)
	{
		this->_animatedTileList.at(i).draw(graphics);
	}
	for (int i = 0; i < this->_enemies.size(); i++)
	{
		this->_enemies.at(i)->draw(graphics);
	}
}

Vector2 Level::parsePosition(const char *positionValue)
{
	std::string stringParsed = this->parseString(positionValue);
	size_t commaPosition = stringParsed.find(',');
	std::string X = stringParsed.substr(0, commaPosition);
	std::string Y = stringParsed.substr(commaPosition + 1);

	int xInt = std::stoi(X);
	int yInt = std::stoi(Y);

	return Vector2(std::ceil(xInt) * globals::SPRITE_SCALE,
								 std::ceil(yInt) * globals::SPRITE_SCALE);
}

std::string Level::parseString(const char *stringValue)
{
	std::stringstream stringStream;
	stringStream << stringValue;

	return stringStream.str();
}

std::vector<Rectangle> Level::checkTileCollisions(const Rectangle &other)
{
	std::vector<Rectangle> others;
	for (int i = 0; i < this->_collisionRects.size(); i++)
	{
		if (this->_collisionRects.at(i).collidesWith(other))
		{
			others.push_back(this->_collisionRects.at(i));
		}
	}
	return others;
}

std::vector<Slope> Level::checkSlopeCollisions(const Rectangle &other)
{
	std::vector<Slope> others;
	for (int i = 0; i < this->_slopes.size(); i++)
	{
		if (this->_slopes.at(i).collidesWith(other))
		{
			others.push_back(this->_slopes.at(i));
		}
	}
	return others;
}

std::vector<Door> Level::checkDoorCollisions(const Rectangle &other)
{
	std::vector<Door> others;
	for (int i = 0; i < this->_doorList.size(); i++)
	{
		if (this->_doorList.at(i).collidesWith(other))
		{
			others.push_back(this->_doorList.at(i));
		}
	}
	return others;
}

std::vector<LevelPassage> Level::checkLevelPassage(const Rectangle &rectangle)
{
	std::vector<LevelPassage> levelPassages;
	for (int i = 0; i < this->_levelPassagesList.size(); i++)
	{

		if (this->_levelPassagesList.at(i).collidesWith(rectangle))
		{
			levelPassages.push_back(this->_levelPassagesList.at(i));
		}
	}
	return levelPassages;
}

std::vector<Enemy *> Level::checkEnemyCollisions(const Rectangle &other)
{
	std::vector<Enemy *> others;
	for (int i = 0; i < this->_enemies.size(); i++)
	{
		if (this->_enemies.at(i)->getBoundingBox().collidesWith(other))
		{
			others.push_back(this->_enemies.at(i));
		}
	}
	return others;
}

const Vector2 Level::getPlayerSpawnPoint() const
{
	return this->_spawnPoint;
}
Vector2 Level::getTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight)
{
	int tilesetWidth, tilesetHeight;
	SDL_QueryTexture(tls.Texture, NULL, NULL, &tilesetWidth, &tilesetHeight);
	int tsxx = gid % (tilesetWidth / tileWidth) - 1;
	tsxx *= tileWidth;
	int tsyy = 0;
	int amt = ((gid - tls.FirstGid) / (tilesetWidth / tileWidth));
	tsyy = tileHeight * amt;
	Vector2 finalTilesetPosition = Vector2(tsxx, tsyy);
	return finalTilesetPosition;
}
