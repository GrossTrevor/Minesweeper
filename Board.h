#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <iterator>
#include "Tile.h"
#include "Mine.h"
#include "Random.h"
#include "Number.h"
#include "Flag.h"
#include "Counter.h"
#include "Button.h"
class Board {
	bool runMode;
	bool debugMode;
	bool testMode;
	int row;
	int col;
	int numMines;

public:
	Board();
	void SetColumns(int);
	void SetRows(int);
	void SetNumMines(int);
	void StartGame();
	void EndGame();
	void DrawTiles(sf::RenderWindow& window, Tile** (&t), Mine* (&m), std::vector<Mine>&, std::vector<Number>&, std::vector<Flag>&, std::vector<Counter>&, std::vector<Button>&, bool, bool, bool);
	void LoadTests(const char* name, std::ifstream&, Tile** (&t), Mine* (&m), std::vector<Mine>&, char** (&store), std::vector<Number>& numbers, std::vector<Flag>& flags);
	void LinkTiles(Tile** (&t));
	void SetTilePositions(Tile** (&t));
	void SetMinePositions(Tile** (&t), Mine* (&m));
	bool CheckMineDuplicates(std::vector<int>& randWidth, std::vector<int>& randHeight);
	int CheckMinePosition(Tile(&t), Mine* (&m));
	int CheckTestMinePosition(Tile(&t), std::vector<Mine>&);
	void SetNumbers(std::vector<Number>&, Tile** (&t));
	void AddFlag(std::vector<Flag>&, Tile(&t));
	void RemoveFlag(std::vector<Flag>&, Tile(&t));
	void CreateCounter(std::vector<Counter>&);
	void CreateButtons(std::vector<Button>&);
	bool GetGameState();
	void SetBoard(std::vector<Number>&, std::vector<Flag>&, Tile** (&t), Mine* (&m));
	void SetDebugMode(bool);
	bool GetDebugMode();
	void SetTestMode(bool b);
	bool GetTestMode();
	bool CheckWin(Tile** (&t), Mine* (&m), std::vector<Flag>&);
	bool CheckTestWin(Tile** (&t), std::vector<Mine>&, std::vector<Flag>&);
};