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

	this->_size = Vector2(levelData["width"], levelData["height"]);
	this->_tileSize = Vector2(levelData["tilewidth"], levelData["tileheight"]);

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
