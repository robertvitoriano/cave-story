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
#include <vector>

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
		for (nlohmann::json animatedTile : tileSet["tiles"])
		{
			AnimatedTileInfo animatedTileInfo;
			animatedTileInfo.StartTileId = animatedTile["id"].get<int>() + firstgid;

			for (nlohmann::json animatedFrame : animatedTile["animation"])
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

				Tileset tileset;

				for (int i = 0; i < this->_tilesets.size(); i++)
				{
					if (this->_tilesets[i].FirstGid <= tileGID)
					{

						tileset = this->_tilesets.at(i);
					}
				}

				int x = 0;
				int y = 0;
				x = tileCounter % width;
				x *= tileWidth;
				y += tileHeight * (tileCounter / width);

				Vector2 finalTilePosition = Vector2(x, y);

				Vector2 finalTilesetPosition = this->getTilesetPosition(tileset, tileGID, tileWidth, tileHeight);

				bool isAnimatedTile = false;
				AnimatedTileInfo animatedTileInfo;
				for (int i = 0; i < this->_animatedTileInfos.size(); i++)
				{
					if (this->_animatedTileInfos.at(i).StartTileId == tileGID)
					{
						animatedTileInfo = this->_animatedTileInfos.at(i);

						isAnimatedTile = true;

						std::vector<Vector2> tilesetPositions;
						for (int i = 0; i < animatedTileInfo.TileIds.size(); i++)
						{
							tilesetPositions.push_back(this->getTilesetPosition(tileset, animatedTileInfo.TileIds.at(i),
																																	tileWidth, tileHeight));
						}
						AnimatedTile tile(tilesetPositions, animatedTileInfo.Duration,
															tileset.Texture, Vector2(tileWidth, tileHeight), finalTilePosition);
						this->_animatedTileList.push_back(tile);
					}
				}
				if (isAnimatedTile)
				{
					continue;
				}

				Tile tile(tileset.Texture, Vector2(tileWidth, tileHeight),
									finalTilesetPosition, finalTilePosition);
				this->_tileList.push_back(tile);
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
			else if (layer["name"] == "doors")
			{
				for (nlohmann::json object : layer["objects"])
				{
					float x = object["x"];
					float y = object["y"];

					float w = object["width"];
					float h = object["height"];

					Rectangle rect = Rectangle(x, y, w, h);
					Vector2 spawnPosition = {0, 0};

					std::string destination;
					for (nlohmann::json property : object["properties"])
					{
						if (property["name"] == "destination")
						{
							destination = property["value"];
						}
						if (property["name"] == "spawn_position")
						{
							spawnPosition = this->parsePosition(property["value"]);
						}
					}
					Door door = Door(rect, destination, spawnPosition);
					this->_doorList.push_back(door);
				}
			}
			else if (layer["name"] == "level_passages")
			{

				for (nlohmann::json object : layer["objects"])
				{
					std::string destination;

					Vector2 spawnPosition = {0, 0};

					float x = object["x"];
					float y = object["y"];

					float w = object["width"];
					float h = object["height"];

					Rectangle rect = Rectangle(x, y, w, h);

					for (nlohmann::json property : object["properties"])
					{
						if (property["name"] == "destination")
						{
							destination = property["value"];
						}
						if (property["name"] == "spawn_position")
						{
							spawnPosition = this->parsePosition(property["value"]);
						}
						LevelPassage levelPassage = LevelPassage(rect, destination, spawnPosition);

						this->_levelPassagesList.push_back(levelPassage);
					}
				}
			}
			else if (layer["name"] == "enemies")
			{

				for (nlohmann::json object : layer["objects"])
				{

					float x = object["x"];
					float y = object["y"];
					if (object["name"] == "bat")
					{
						this->_enemies.push_back(new Bat(graphics, Vector2(std::floor(x) * globals::SPRITE_SCALE,
																															 std::floor(y) * globals::SPRITE_SCALE)));
					}
				}
			}
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

Vector2 Level::parsePosition(std::string positionString)
{
	size_t commaPosition = positionString.find(',');
	std::string xStr = positionString.substr(0, commaPosition);
	std::string yStr = positionString.substr(commaPosition + 1);

	int xInt = std::stoi(xStr);
	int yInt = std::stoi(yStr);

	return Vector2(xInt, yInt);
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
Vector2 Level::getTilesetPosition(Tileset tileset, int gid, int tileWidth, int tileHeight)
{
	int tilesetWidth, tilesetHeight;

	SDL_QueryTexture(tileset.Texture, NULL, NULL, &tilesetWidth, &tilesetHeight);
	int numberOfTilesPerRow = tilesetWidth / tileWidth;

	int relativeGid = gid - tileset.FirstGid;
	int columnPosition = 0;

	if (tileset.FirstGid <= 1)
	{
		columnPosition = gid % numberOfTilesPerRow - 1;
	}
	else
	{
		columnPosition = (relativeGid % numberOfTilesPerRow);
	}

	int rowPosition = (relativeGid / numberOfTilesPerRow);

	int tileXPosition = columnPosition * tileWidth;
	int tileYPosition = tileHeight * rowPosition;

	return Vector2(tileXPosition, tileYPosition);
}
