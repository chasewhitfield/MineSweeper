#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "Tile.h"

using namespace std;

class Board
{
private:
	int flags, bombs, shown, columns, rows;
	string status;
	bool debug, winner;
public:
	Board();
	Board(int columns, int rows);
	void findNumNeigbors(vector<Tile> &tiles, int index, int columns, int rows);
	void showMines(vector<Tile>& tiles);
	string showStatus();
	void setStatus(string s);
	void setDebug(bool b);
	bool getDebug();
	void clickAlgorithm(vector<Tile> &tiles, int index);
	void setColumn(int x);
	int getColumn();
	void setRow(int x);
	int getRow();
	void setMines(int x);
	int getMines();
	void setFlags(int x);
	int getFlags();
	void checkWinner(vector<Tile> &tiles);

	//Show all bombs
	//any game buttons tbh
	//flag count
	//draw method overloaded
};