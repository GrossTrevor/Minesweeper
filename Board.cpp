#include "Board.h"
using namespace std;

//Builds the default Board
Board::Board() {
	runMode = true;
	debugMode = false;
	testMode = false;
	row = 0;
	col = 0;
	numMines = 0;
}

void Board::SetColumns(int c) {
	col = c;
}

void Board::SetRows(int r) {
	row = r;
}

void Board::SetNumMines(int m) {
	numMines = m;
}

void Board::StartGame() {
	this->runMode = true;
}

void Board::EndGame() {
	this->runMode = false;
}

//Draws everything and regularly updates from main.cpp
void Board::DrawTiles(sf::RenderWindow& window, Tile** (&t), Mine* (&m), vector<Mine>& testMines, vector<Number>& nums, vector<Flag>& flags, vector<Counter>& counters, vector<Button>& buttons, bool continueGame, bool debugMode, bool testMode) {
	int index = 0;
	//Drawing the Board by getting each Sprite from each Tile on a regular game
	if (continueGame && !testMode) {
		for (int r = 0; r < row; r++) {
			for (int c = 0; c < col; c++) {
				index = CheckMinePosition(t[r][c], m);
				if (index != -1 && t[r][c].IsOpen()) {
					window.draw(t[r][c].GetSprite());
					window.draw(m[index].GetSprite());

				}
				else {
					window.draw(t[r][c].GetSprite());
					for (unsigned int n = 0; n < nums.size(); n++) {
						if (t[r][c].IsOpen()) {
							if (nums[n].tile.getPosition().x == t[r][c].tile.getPosition().x && nums[n].tile.getPosition().y == t[r][c].tile.getPosition().y)
								window.draw(nums[n].GetSprite());
						}
					}
				}
			}
		}
		if (CheckWin(t, m, flags))
			window.draw(buttons[2].GetSprite());
		if (!CheckWin(t, m, flags))
			window.draw(buttons[0].GetSprite());
	}
	//...on a game using a test board
	if (testMode) {
		for (int r = 0; r < row; r++) {
			for (int c = 0; c < col; c++) {
				index = CheckTestMinePosition(t[r][c], testMines);
				if ((index != -1 && debugMode) || (index != -1 && !continueGame)) {
					window.draw(t[r][c].GetSprite());
					window.draw(testMines[index].GetSprite());
				}
				else if (index != -1 && !debugMode)
					window.draw(t[r][c].GetSprite());
				else {
					window.draw(t[r][c].GetSprite());
					for (unsigned int n = 0; n < nums.size(); n++) {
						if (t[r][c].IsOpen()) {
							if (nums[n].tile.getPosition().x == t[r][c].tile.getPosition().x && nums[n].tile.getPosition().y == t[r][c].tile.getPosition().y)
								window.draw(nums[n].GetSprite());
						}
					}
				}
			}
		}
		if (continueGame && !CheckTestWin(t, testMines, flags))
			window.draw(buttons[0].GetSprite());
		if (continueGame && CheckTestWin(t, testMines, flags))
			window.draw(buttons[2].GetSprite());
		if (!continueGame)
			window.draw(buttons[1].GetSprite());
	}
	//...when debugMode is active
	if ((!continueGame || debugMode) && !testMode) {
		for (int r = 0; r < row; r++) {
			for (int c = 0; c < col; c++) {
				index = CheckMinePosition(t[r][c], m);
				if (index != -1) {
					window.draw(t[r][c].GetSprite());
					window.draw(m[index].GetSprite());
				}
				else {
					window.draw(t[r][c].GetSprite());
					for (unsigned int n = 0; n < nums.size(); n++) {
						if (t[r][c].IsOpen()) {
							if (nums[n].tile.getPosition().x == t[r][c].tile.getPosition().x && nums[n].tile.getPosition().y == t[r][c].tile.getPosition().y)
								window.draw(nums[n].GetSprite());
						}
					}
				}
			}
		}
		if (!continueGame)
			window.draw(buttons[1].GetSprite());
		if (CheckWin(t, m, flags))
			window.draw(buttons[2].GetSprite());
	}
	//Drawing and calculating the number of Flags then adjusting the Counter
	for (unsigned int i = 0; i < flags.size(); i++)
		window.draw(flags[i].GetSprite());
	if (flags.size() <= (unsigned)numMines) {
		index = numMines - flags.size();
		counters[0].SetPosition(0, (float)row * 32);
		window.draw(counters[0].GetSprite());
		counters[index / 10].SetPosition(21, (float)row * 32);
		window.draw(counters[index / 10].GetSprite());
		counters[index % 10].SetPosition(42, (float)row * 32);
		window.draw(counters[index % 10].GetSprite());
	}
	else if (flags.size() > (unsigned)numMines) {
		counters[10].SetPosition(0, (float)row * 32);
		window.draw(counters[10].GetSprite());
		index = flags.size() - numMines;
		if (index >= numMines * 2) {
			counters[index / 100].SetPosition(21, (float)row * 32);
			window.draw(counters[index / 100].GetSprite());
			counters[(index / 10) % 10].SetPosition(42, (float)row * 32);
			window.draw(counters[(index / 10) % 10].GetSprite());
			counters[index % 10].SetPosition(63, (float)row * 32);
			window.draw(counters[index % 10].GetSprite());
		}
		else {
			counters[0].SetPosition(21, (float)row * 32);
			window.draw(counters[0].GetSprite());
			counters[index / 10].SetPosition(42, (float)row * 32);
			window.draw(counters[index / 10].GetSprite());
			counters[index % 10].SetPosition(63, (float)row * 32);
			window.draw(counters[index % 10].GetSprite());
		}
	}
	//Drawing the Buttons below the board
	window.draw(buttons[3].GetSprite());
	window.draw(buttons[4].GetSprite());
	window.draw(buttons[5].GetSprite());
	window.draw(buttons[6].GetSprite());
}

//Loading the testboard# data and imposing it into the Board
void Board::LoadTests(const char* name, ifstream& f, Tile** (&t), Mine* (&m), vector<Mine>& mines, char** (&store), std::vector<Number>& numbers, std::vector<Flag>& flags) {
	f.open(name);
	mines.clear();
	numbers.clear();
	flags.clear();
	vector<Tile> mineTiles;
	string line;
	int count = 0;
	while (getline(f, line, '\n')) {
		for (int c = 0; c < col; c++)
			store[count][c] = line[c];
		count++;
	}
	//Finding all the Mines from the testboard# file and adding them to a vector
	for (int r = 0; r < row; r++) {
		for (int c = 0; c < col; c++) {
			t[r][c].ResetTile();
			if (store[r][c] == '1') {
				t[r][c].SetMine();
				mineTiles.push_back(t[r][c]);
			}
		}
	}
	//Taking the Mines from the previous vector and addint them to the main Mine vector
	for (unsigned int i = 0; i < mineTiles.size(); i++) {
		Mine mn;
		mn.tile.setPosition(mineTiles[i].tile.getPosition().x, mineTiles[i].tile.getPosition().y);
		mines.push_back(mn);
	}
	SetNumbers(numbers, t);
	mineTiles.clear();
}

//Setting the pointers from teach Tile so it accurately points at its neighbors
void Board::LinkTiles(Tile** (&t)) {
	for (int r = 0; r < row; r++) {
		for (int c = 0; c < col; c++) {
			if (r == 0) {
				t[r][c].neighbors[0] = nullptr;
				t[r][c].neighbors[1] = nullptr;
				t[r][c].neighbors[7] = nullptr;
				if (c == 0) {
					t[r][c].neighbors[2] = &t[0][1];
					t[r][c].neighbors[3] = &t[1][1];
					t[r][c].neighbors[4] = &t[1][0];
					t[r][c].neighbors[5] = nullptr;
					t[r][c].neighbors[6] = nullptr;
				}
				if (c != 0 && c != col - 1) {
					t[r][c].neighbors[2] = &t[r][c + 1];
					t[r][c].neighbors[3] = &t[r + 1][c + 1];
					t[r][c].neighbors[4] = &t[r + 1][c];
					t[r][c].neighbors[5] = &t[r + 1][c - 1];
					t[r][c].neighbors[6] = &t[r][c - 1];
				}
				if (c == col - 1) {
					t[r][c].neighbors[6] = &t[r][c - 1];
					t[r][c].neighbors[5] = &t[r + 1][c - 1];
					t[r][c].neighbors[4] = &t[r + 1][c];
					t[r][c].neighbors[2] = nullptr;
					t[r][c].neighbors[3] = nullptr;
				}
			}
			else if (r != 0 && r != row - 1) {
				if (c == 0) {
					t[r][c].neighbors[0] = &t[r - 1][c];
					t[r][c].neighbors[1] = &t[r - 1][c + 1];
					t[r][c].neighbors[2] = &t[r][c + 1];
					t[r][c].neighbors[3] = &t[r + 1][c + 1];
					t[r][c].neighbors[4] = &t[r + 1][c];
					t[r][c].neighbors[5] = nullptr;
					t[r][c].neighbors[6] = nullptr;
					t[r][c].neighbors[7] = nullptr;
				}
				else if (c == col - 1) {
					t[r][c].neighbors[0] = &t[r - 1][c];
					t[r][c].neighbors[1] = nullptr;
					t[r][c].neighbors[2] = nullptr;
					t[r][c].neighbors[3] = nullptr;
					t[r][c].neighbors[4] = &t[r + 1][c];
					t[r][c].neighbors[5] = &t[r + 1][c - 1];
					t[r][c].neighbors[6] = &t[r][c - 1];
					t[r][c].neighbors[7] = &t[r - 1][c - 1];
				}
				else if (c != 0 && c != col - 1) {
					t[r][c].neighbors[0] = &t[r - 1][c];
					t[r][c].neighbors[1] = &t[r - 1][c + 1];
					t[r][c].neighbors[2] = &t[r][c + 1];
					t[r][c].neighbors[3] = &t[r + 1][c + 1];
					t[r][c].neighbors[4] = &t[r + 1][c];
					t[r][c].neighbors[5] = &t[r + 1][c - 1];
					t[r][c].neighbors[6] = &t[r][c - 1];
					t[r][c].neighbors[7] = &t[r - 1][c - 1];
				}
			}
			else if (r == row - 1) {
				t[r][c].neighbors[3] = nullptr;
				t[r][c].neighbors[4] = nullptr;
				t[r][c].neighbors[5] = nullptr;
				if (c == 0) {
					t[r][c].neighbors[2] = &t[r][c + 1];
					t[r][c].neighbors[1] = &t[r - 1][c + 1];
					t[r][c].neighbors[0] = &t[r - 1][c];
					t[r][c].neighbors[6] = nullptr;
					t[r][c].neighbors[7] = nullptr;
				}
				if (c != 0 && c != col - 1) {
					t[r][c].neighbors[0] = &t[r - 1][c];
					t[r][c].neighbors[1] = &t[r - 1][c + 1];
					t[r][c].neighbors[2] = &t[r][c + 1];
					t[r][c].neighbors[6] = &t[r][c - 1];
					t[r][c].neighbors[7] = &t[r - 1][c - 1];

				}
				if (c == col - 1) {
					t[r][c].neighbors[0] = &t[r - 1][c];
					t[r][c].neighbors[7] = &t[r - 1][c - 1];
					t[r][c].neighbors[6] = &t[r][c - 1];
					t[r][c].neighbors[2] = nullptr;
					t[r][c].neighbors[1] = nullptr;
				}
			}
		}
	}
}

//Laying each Tile on the board
void Board::SetTilePositions(Tile** (&t)) {
	float x = 0.0f;
	float y = 0.0f;
	for (int r = 0; r < row; r++) {
		for (int c = 0; c < col; c++) {
			t[r][c].SetPosition(x, y);
			x += 32.0f;
		}
		x = 0;
		y += 32.0f;
	}
}

//Randomizing the location of each Mine throughout the Board
void Board::SetMinePositions(Tile** (&t), Mine* (&m)) {
	vector<int> randWidth;
	vector<int> randHeight;
	for (int i = 0; i < numMines; i++) {
		int height = Random::Int(0, row - 1);
		randHeight.push_back(height);
		int width = Random::Int(0, col - 1);
		randWidth.push_back(width);
	}
	if (CheckMineDuplicates(randWidth, randHeight)) {
		randWidth.clear();
		randHeight.clear();
		SetMinePositions(t, m);
	}
	else {
		for (int i = 0; i < numMines; i++) {
			m[i].SetPosition(t[randHeight[i]][randWidth[i]].tile.getPosition().x, t[randHeight[i]][randWidth[i]].tile.getPosition().y);
			t[randHeight[i]][randWidth[i]].SetMine();
		}
	}
}

//Checks a given Tile to see if a Mine is present there
int Board::CheckMinePosition(Tile(&t), Mine* (&m)) {
	for (int i = 0; i < numMines; i++) {
		if (t.tile.getPosition().x == m[i].tile.getPosition().x && t.tile.getPosition().y == m[i].tile.getPosition().y) {
			return i;
		}
	}
	return -1;
}

//Checks a given Tile to see if a Mine is present using the testMine vector
int Board::CheckTestMinePosition(Tile& t, vector<Mine>& testMines) {
	for (unsigned int i = 0; i < testMines.size(); i++) {
		if (t.tile.getPosition().x == testMines[i].tile.getPosition().x && t.tile.getPosition().y == testMines[i].tile.getPosition().y) {
			return i;
		}
	}
	return -1;
}

//Checks if multiple Mines are in the same location
bool Board::CheckMineDuplicates(vector<int>& randWidth, vector<int>& randHeight) {
	for (unsigned int i = 0; i < randHeight.size(); i++) {
		for (unsigned int j = i + 1; j < randHeight.size(); j++) {
			if (randHeight[i] == randHeight[j] && i != j) {
				if (randWidth[i] == randWidth[j]) {
					return true;
				}
			}
		}
	}
	return false;
}


//Setting which Tiles will contain Numbers based on the surrounding Mines
void Board::SetNumbers(vector<Number>& nums, Tile** (&t)) {
	int count = 0;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			count = t[i][j].CheckNeighbors();
			if (count != 0) {
				if (!t[i][j].IsMine()) {
					Number n(count);
					n.SetPosition(t[i][j].tile.getPosition().x, t[i][j].tile.getPosition().y);
					nums.push_back(n);
				}
			}
		}
	}
}

//Adds a Flag into a Tile
void Board::AddFlag(std::vector<Flag>& flags, Tile& t) {
	Flag f;
	f.SetPosition(t.tile.getPosition().x, t.tile.getPosition().y);
	flags.push_back(f);
	t.GiveFlag();
}

//Removes a Flag from a Tile
void Board::RemoveFlag(std::vector<Flag>& flags, Tile& t) {
	for (unsigned int i = 0; i < flags.size(); i++) {
		if (flags[i].tile.getPosition().x == t.tile.getPosition().x && flags[i].tile.getPosition().y == t.tile.getPosition().y)
			flags.erase(flags.begin() + i);
	}
	t.RemoveFlag();
}

//Builds the Counter Vector for each segment of the digits file
void Board::CreateCounter(std::vector<Counter>& counters) {
	for (int i = 0; i <= 10; i++) {
		Counter c(i);
		counters.push_back(c);
	}
}

//Sets files and positions for each of the Buttons and adds them to a vector
void Board::CreateButtons(std::vector<Button>& buttons) {
	Button a("face_happy");
	a.SetPosition(368, (float)row * 32);
	Button b("face_lose");
	b.SetPosition(368, (float)row * 32);
	Button c("face_win");
	c.SetPosition(368, (float)row * 32);
	Button d("debug");
	d.SetPosition(496, (float)row * 32);
	Button e("test_1");
	e.SetPosition(560, (float)row * 32);
	Button f("test_2");
	f.SetPosition(624, (float)row * 32);
	Button g("test_3");
	g.SetPosition(688, (float)row * 32);

	buttons.push_back(a);
	buttons.push_back(b);
	buttons.push_back(c);
	buttons.push_back(d);
	buttons.push_back(e);
	buttons.push_back(f);
	buttons.push_back(g);
}

bool Board::GetGameState() {
	return this->runMode;
}

//Resets the Board, then sets the Board with new vectors of Mines and Numbers and sets runMode to true
void Board::SetBoard(std::vector<Number>& numbers, std::vector<Flag>& flags, Tile** (&t), Mine* (&m)) {
	numbers.clear();
	flags.clear();
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++)
			t[i][j].ResetTile();
	}
	SetMinePositions(t, m);
	SetNumbers(numbers, t);
	StartGame();
}

void Board::SetDebugMode(bool debug) {
	this->debugMode = debug;
}

bool Board::GetDebugMode() {
	return this->debugMode;
}

void Board::SetTestMode(bool b) {
	this->testMode = b;
}

bool Board::GetTestMode() {
	return this->testMode;
}

//Checks win conditions
bool Board::CheckWin(Tile** (&t), Mine* (&m), vector<Flag>& flags) {
	int mineCount = 0;
	//Checks if each Tile is open and does not contain a Mine
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (t[i][j].IsOpen() && CheckMinePosition(t[i][j], m) == -1)
				mineCount++;
		}
	}
	//If the total number of Tile minus the number of Mines is equal to the number of tiles open...
	if (mineCount == (row * col) - numMines) {
		flags.clear();
		//...add flags to all Mined Tiles and return true
		for (int i = 0; i < numMines; i++) {
			Flag f;
			f.SetPosition(m[i].GetSprite().getPosition().x, m[i].GetSprite().getPosition().y);
			flags.push_back(f);
		}
		return true;
	}
	else {
		return false;
	}
}

//Checks win conditions while a testboard is loaded
bool Board::CheckTestWin(Tile** (&t), vector<Mine>& testMines, vector<Flag>& flags) {
	int mineCount = 0;
	//Checks if each Tile is open and does not contain a Mine
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (t[i][j].IsOpen() && CheckTestMinePosition(t[i][j], testMines) == -1)
				mineCount++;
		}
	}
	//If the total number of Tile minus the number of Mines is equal to the number of tiles open...
	if (mineCount == (row * col) - testMines.size()) {
		flags.clear();
		//...add flags to all Mined Tiles and return true
		for (unsigned int i = 0; i < testMines.size(); i++) {
			Flag f;
			f.SetPosition(testMines[i].GetSprite().getPosition().x, testMines[i].GetSprite().getPosition().y);
			flags.push_back(f);
		}
		return true;
	}
	else {
		return false;
	}
}