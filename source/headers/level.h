#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>

#include "globals.h"
#include "tile.h"
#include "rectangle.h"
#include "slope.h"
#include "animatedtile.h"
#include "door.h"
#include "LevelPassage.h"
#include <iostream>
#include <nlohmann/json.hpp>

class Graphics;
class Enemy;
class Bat;
class Player;
struct SDL_Texture;
struct SDL_Rect;
struct Tileset;

class Level
{
public:
	Level();
	Level(std::string mapName, Graphics &graphics);
	~Level();
	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics, Player &player);

	std::vector<Rectangle> checkTileCollisions(const Rectangle &other);
	std::vector<Slope> checkSlopeCollisions(const Rectangle &other);
	std::vector<Door> checkDoorCollisions(const Rectangle &other);
	std::vector<LevelPassage> checkLevelPassage(const Rectangle &otherRectangle);
	std::vector<Enemy *> checkEnemyCollisions(const Rectangle &other);

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
	std::vector<AnimatedTileInfo> _animatedTileInfos;

	std::vector<Door> _doorList;
	std::vector<LevelPassage> _levelPassagesList;
	std::vector<Enemy *> _enemies;
	bool _isLevel3D;

	/* void loadMap
	 * Loads a map
	 */
	void loadMap(std::string mapName, Graphics &graphics);

	Vector2 getTilesetPosition(Tileset tileset, int gid, int tileWidth, int tileHeight);
	Vector2 parsePosition(std::string positionString);
	void parseTileLayer(nlohmann::json layer);
	std::string parseString(const char *stringValue);
};

// Tileset structure
struct Tileset
{
	SDL_Texture *Texture;
	int FirstGid;
	Tileset()
	{
		this->FirstGid = -1;
	}
	Tileset(SDL_Texture *texture, int firstGid)
	{
		this->Texture = texture;
		this->FirstGid = firstGid;
	}
};

#endif
