#pragma once
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
using sf::Sprite;

class Tile {
	bool isMine;
	bool isOpen;
	bool isFlag;
	int nextMines;

public:
	Tile* neighbors[8];
	Sprite tile;
	Tile();
	void SetSprite(const char* name);
	Sprite& GetSprite();
	void SetPosition(float x, float y);
	void OpenTile();
	void SetMine();
	bool IsMine();
	bool IsOpen();
	int CheckNeighbors();
	void OpenNeighbors();
	void GiveFlag();
	bool IsFlag();
	void RemoveFlag();
	void ResetTile();
};