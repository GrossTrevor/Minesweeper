#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include "TextureManager.h"
#include "Random.h"
#include "Mine.h"
#include "Board.h"
#include "Flag.h"
#include "Counter.h"
using namespace std;
using sf::RenderWindow;
using sf::VideoMode;
using sf::Mouse;
using sf::Event;
using sf::Vector2f;

int main() {
    //Read Config.cfg file
    ifstream cfg;
    cfg.open("boards/config.cfg");
    string line;
    getline(cfg, line, '\n');
    int col = stoi(line);
    getline(cfg, line, '\n');
    int row = stoi(line);
    getline(cfg, line, '\n');
    int numMines = stoi(line);

    //Set number of columns, rows, and mines
    Board gameBoard;
    gameBoard.SetColumns(col);
    gameBoard.SetRows(row);
    gameBoard.SetNumMines(numMines);

    RenderWindow window(VideoMode(col * 32, (row * 32) + 100), "Minesweeper");

    //Allocate 2D Arrays for Tile and char (temp storage) and 1D Array for Mine
    Tile** tiles = new Tile * [row];
    for (int r = 0; r < row; r++)
        tiles[r] = new Tile[col];

    Mine* mines = new Mine[numMines];

    char** store = new char* [row];
    for (int r = 0; r < row; r++)
        store[r] = new char[col];

    //Initialize data structures to be used
    Event event;
    vector<Mine> testMines;
    vector<Number> numbers;
    vector<Flag> flags;
    vector<Counter> counters;
    vector<Button> buttons;

    ifstream test1;
    ifstream test2;
    ifstream test3;

    //Create and setup gameBoard elements
    gameBoard.CreateButtons(buttons);
    gameBoard.CreateCounter(counters);
    gameBoard.SetTilePositions(tiles);
    gameBoard.LinkTiles(tiles);
    gameBoard.SetMinePositions(tiles, mines);
    gameBoard.SetNumbers(numbers, tiles);

    //The game
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            switch (event.type) {
            case Event::Closed:
                window.close();
                break;

            //Tile is clicked
            case Event::MouseButtonPressed:
                if (Mouse::isButtonPressed(Mouse::Left)) {
                    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
                    for (int r = 0; r < row; r++) {
                        for (int c = 0; c < col; c++) {
                            if (tiles[r][c].tile.getGlobalBounds().contains(mousePos) && !tiles[r][c].IsFlag() && gameBoard.GetGameState()) {
                                tiles[r][c].OpenTile();
                                if (tiles[r][c].IsMine())
                                    gameBoard.EndGame();
                                else
                                    tiles[r][c].OpenNeighbors();
                            }
                        }
                    }
                    //Happy face is clicked
                    if (buttons[1].GetSprite().getGlobalBounds().contains(mousePos)) {
                        gameBoard.SetBoard(numbers, flags, tiles, mines);
                        gameBoard.SetTestMode(false);
                        test1.close();
                        test2.close();
                        test3.close();
                    }
                    //Toggle debugMode
                    else if (buttons[3].GetSprite().getGlobalBounds().contains(mousePos) && !gameBoard.GetDebugMode())
                        gameBoard.SetDebugMode(true);
                    else if (buttons[3].GetSprite().getGlobalBounds().contains(mousePos) && gameBoard.GetDebugMode())
                        gameBoard.SetDebugMode(false);
                    //Load testboard1
                    else if (buttons[4].GetSprite().getGlobalBounds().contains(mousePos)) {
                        gameBoard.SetTestMode(true);
                        gameBoard.StartGame();
                        test2.close();
                        test3.close();
                        gameBoard.LoadTests("boards/testboard1.brd", test1, tiles, mines, testMines, store, numbers, flags);

                    }
                    //Load testboard2
                    else if (buttons[5].GetSprite().getGlobalBounds().contains(mousePos)) {
                        gameBoard.SetTestMode(true);
                        gameBoard.StartGame();
                        test1.close();
                        test3.close();
                        gameBoard.LoadTests("boards/testboard2.brd", test2, tiles, mines, testMines, store, numbers, flags);

                    }
                    //Load testboard3
                    else if (buttons[6].GetSprite().getGlobalBounds().contains(mousePos)) {
                        gameBoard.SetTestMode(true);
                        gameBoard.StartGame();
                        test1.close();
                        test2.close();
                        gameBoard.LoadTests("boards/testboard3.brd", test3, tiles, mines, testMines, store, numbers, flags);
                    }
                }
                //Flag a Tile
                if (Mouse::isButtonPressed(Mouse::Right)) {
                    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
                    for (int r = 0; r < row; r++) {
                        for (int c = 0; c < col; c++) {
                            if (tiles[r][c].tile.getGlobalBounds().contains(mousePos) && !tiles[r][c].IsFlag() && !tiles[r][c].IsOpen() && gameBoard.GetGameState())
                                gameBoard.AddFlag(flags, tiles[r][c]);
                            else if (tiles[r][c].tile.getGlobalBounds().contains(mousePos) && !tiles[r][c].IsOpen() && tiles[r][c].IsFlag() && gameBoard.GetGameState())
                                gameBoard.RemoveFlag(flags, tiles[r][c]);
                        }
                    }
                }
                break;
            }
        }
        //Draw the Tiles according to what is clicked up above
        window.clear();
        gameBoard.DrawTiles(window, tiles, mines, testMines, numbers, flags, counters, buttons, gameBoard.GetGameState(), gameBoard.GetDebugMode(), gameBoard.GetTestMode());
        window.display();
    }
    TextureManager::Clear();
    return 0;
}
