#include "level.h"
#include "graphics.h"
#include "globals.h"
#include "utils.h"
#include "player.h"
#include "enemy.h"
#include "bat.h"
#include <nlohmann/json.hpp>

#include <SDL2/SDL.h>
#include "globals.h"

#include <sstream>
#include <algorithm>
#include <cmath>
#include "LevelPassage.h"
#include <fstream>

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
	std::string filePath = "maps/" + mapName + ".json";
	std::ifstream inputFile(filePath);

	if (!inputFile.is_open())
	{
		std::cerr << "Error: Could not open file " << filePath << std::endl;
	}

	nlohmann::json levelData;
	try
	{
		inputFile >> levelData;
	}
	catch (const nlohmann::json::parse_error &e)
	{
		std::cerr << "Error parsing JSON: " << e.what() << std::endl;
	}
	inputFile.close();
	int tileWidth = levelData["tilewidth"];
	int tileHeight = levelData["tileheight"];
	int width = 20;
	int height = levelData["height"];
	this->_size = Vector2(width, height);
	this->_tileSize = Vector2(tileWidth, tileHeight);

	for (nlohmann::json tileSet : levelData["tilesets"])
	{
		int firstgid = tileSet["firstgid"];
		std::string imagePath = tileSet["image"];
		SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(imagePath.erase(0, 3)));
		this->_tilesets.push_back(Tileset(tex, firstgid));
		for (nlohmann::json animatedTiles : tileSet["tiles"])
		{
			AnimatedTileInfo animatedTileInfo;
			animatedTileInfo.TileIds.push_back(animatedTiles["id"].get<int>() + firstgid);
			for (nlohmann::json animatedFrame : animatedTiles["animation"])
			{
				animatedTileInfo.TileIds.push_back(animatedFrame["tileid"].get<int>() + firstgid);
				animatedTileInfo.Duration = animatedFrame["duration"];
			}
			this->_animatedTileInfos.push_back(animatedTileInfo);
		}
	}
	for (nlohmann::json layer : levelData["layers"])
	{
		if (layer["type"] == "tilelayer")
		{

			int tileCounter = 0;
			for (nlohmann::json tileGID : layer["data"])
			{
				if (tileGID == 0)
					continue;

				// Get the tileset for this specific gid

				Tileset tls;
				int closest = 0;
				for (int i = 0; i < this->_tilesets.size(); i++)
				{
					if (this->_tilesets[i].FirstGid <= tileGID)
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
				}

				// Get the position of the tile in the level
				int xx = 0;
				int yy = 0;
				xx = tileCounter % width;
				xx *= tileWidth;
				yy += tileHeight * (tileCounter / width);
				Vector2 finalTilePosition = Vector2(xx, yy);

				// Calculate the position of the tile in the tileset
				Vector2 finalTilesetPosition = this->getTilesetPosition(tls, tileGID, tileWidth, tileHeight);

				// Build the actual tile and add it to the level's tile list
				bool isAnimatedTile = false;
				AnimatedTileInfo ati;
				for (int i = 0; i < this->_animatedTileInfos.size(); i++)
				{
					if (this->_animatedTileInfos.at(i).StartTileId == tileGID)
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
			}
		}
		if (layer["type"] == "objectgroup")
		{
			if (layer["name"] == "collisions")
			{
				for (nlohmann::json object : layer["objects"])
				{
					float x, y, width, height;
					x = object["x"];
					y = object["y"];
					width = object["width"];
					height = object["height"];
					this->_collisionRects.push_back(Rectangle(
							std::ceil(x) * globals::SPRITE_SCALE,
							std::ceil(y) * globals::SPRITE_SCALE,
							std::ceil(width) * globals::SPRITE_SCALE,
							std::ceil(height) * globals::SPRITE_SCALE));
				}
			}
			else if (layer["name"] == "spawn points")
			{
				for (nlohmann::json object : layer["objects"])
				{

					float x = object["x"];
					float y = object["y"];
					std::string name = object["name"];
					if (name == "player")
					{
						this->_spawnPoint = Vector2(std::ceil(x) * globals::SPRITE_SCALE,
																				std::ceil(y) * globals::SPRITE_SCALE);
					}
				}
			}
			else if (layer["name"] == "slopes")
			{
				for (nlohmann::json object : layer["objects"])
				{
					std::vector<Vector2> points;
					Vector2 p1;
					p1 = Vector2(std::ceil(object["x"].get<double>()), std::ceil(object["y"].get<double>()));

					nlohmann::json polylinePoints = object["polyline"];
					for (nlohmann::json point : polylinePoints)
					{
						points.push_back(Vector2(point["x"], point["y"]));
					}

					for (int i = 0; i < points.size(); i += 2)
					{
						this->_slopes.push_back(Slope(
								Vector2((p1.x + points.at(i < 2 ? i : i - 1).x) * globals::SPRITE_SCALE,
												(p1.y + points.at(i < 2 ? i : i - 1).y) * globals::SPRITE_SCALE),
								Vector2((p1.x + points.at(i < 2 ? i + 1 : i).x) * globals::SPRITE_SCALE,
												(p1.y + points.at(i < 2 ? i + 1 : i).y) * globals::SPRITE_SCALE)));
					}
				}
			}
		}
		// else if (objectLayerName == "doors")
		// {
		// 	XMLElement *pObject = pObjectGroup->FirstChildElement("object");

		// 	while (pObject)
		// 	{
		// 		float x = object("x");
		// 		float y = object("y");
		// 		float w = object("width");
		// 		float h = object("height");
		// 		Rectangle rect = Rectangle(x, y, w, h);
		// 		Vector2 spawnPosition = {0, 0};
		// 		std::string destination;
		// 		XMLElement *pProperties = pObject->FirstChildElement("properties");

		// 		while (pProperties)
		// 		{
		// 			XMLElement *pProperty = pProperties->FirstChildElement("property");

		// 			while (pProperty)
		// 			{
		// 				const char *name = pProperty->Attribute("name");
		// 				std::string propertyName = this->parseString(name);
		// 				if (propertyName == "destination")
		// 				{
		// 					const char *value = pProperty->Attribute("value");
		// 					destination = this->parseString(value);
		// 				}
		// 				if (propertyName == "spawn_position")
		// 				{
		// 					const char *value = pProperty->Attribute("value");
		// 					spawnPosition = this->parsePosition(value);
		// 				}
		// 				pProperty = pProperty->NextSiblingElement("property");
		// 			}
		// 			pProperties = pProperties->NextSiblingElement("properties");
		// 		}

		// 		Door door = Door(rect, destination, spawnPosition);
		// 		this->_doorList.push_back(door);
		// 		pObject = pObject->NextSiblingElement("object");
		// 	}
		// }
		// else if (objectLayerName == "level_passages")
		// {
		// 	XMLElement *pObject = pObjectGroup->FirstChildElement("object");

		// 	while (pObject)
		// 	{
		// 		std::string destination;
		// 		Vector2 spawnPosition = {0, 0};

		// 		float x = object("x");
		// 		float y = object("y");
		// 		float w = object("width");
		// 		float h = object("height");
		// 		Rectangle rect = Rectangle(x, y, w, h);

		// 		XMLElement *pProperties = pObject->FirstChildElement("properties");

		// 		while (pProperties)
		// 		{
		// 			XMLElement *pProperty = pProperties->FirstChildElement("property");

		// 			while (pProperty)
		// 			{
		// 				const char *name = pProperty->Attribute("name");
		// 				std::string propertyName = this->parseString(name);
		// 				if (propertyName == "destination")
		// 				{
		// 					const char *value = pProperty->Attribute("value");
		// 					destination = this->parseString(value);
		// 				}
		// 				if (propertyName == "spawn_position")
		// 				{
		// 					const char *value = pProperty->Attribute("value");
		// 					spawnPosition = this->parsePosition(value);
		// 				}
		// 				pProperty = pProperty->NextSiblingElement("property");
		// 			}

		// 			pProperties = pProperties->NextSiblingElement("properties");
		// 		}

		// 		LevelPassage levelPassage = LevelPassage(rect, destination, spawnPosition);
		// 		this->_levelPassagesList.push_back(levelPassage);
		// 		pObject = pObject->NextSiblingElement("object");
		// 	}
		// }
		// else if (objectLayerName == "enemies")
		// {
		// 	float x, y;
		// 	XMLElement *pObject = pObjectGroup->FirstChildElement("object");

		// 	while (pObject)
		// 	{
		// 		x = object("x");
		// 		y = object("y");
		// 		const char *name = pObject->Attribute("name");
		// 		std::string enemyName = this->parseString(name);
		// 		if (enemyName == "bat")
		// 		{
		// 			this->_enemies.push_back(new Bat(graphics, Vector2(std::floor(x) * globals::SPRITE_SCALE,
		// 																												 std::floor(y) * globals::SPRITE_SCALE)));
		// 		}

		// 		pObject = pObject->NextSiblingElement("object");
		// 	}
		// }

		// pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
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
