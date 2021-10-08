#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "Board.h"

using namespace std;

Board::Board(int columns, int rows)
{
	this->columns = columns;
	this->rows = rows;
	this->flags = 0;
	this->bombs = 0;
	this->shown = 0;
	this->status = "game";
	this->debug = false;
	this->winner = false;
}

void Board::findNumNeigbors(vector<Tile> &t, int index, int columns, int rows)
{
	int count = 0;
	vector<int> w = t.at(index).getIndexes();
	for (unsigned int n = 0; n < w.size(); n++)
	{
		if (t.at(w.at(n)).Mine())
		{
			count++;
		}
	}
	t.at(index).setNumNeighbors(count);
}

void Board::showMines(vector<Tile> &t)
{
	for (unsigned int n = 0; n < t.size(); n++)
	{
		if (t.at(n).Mine())
		{
			t.at(n).setClicked(true);
		}
	}
}

string Board::showStatus()
{
	return this->status;
}

void Board::setStatus(string s)
{
	this->status = s;
}

void Board::setDebug(bool b)
{
	this->debug = b;
}

bool Board::getDebug()
{
	return this->debug;
}

void Board::clickAlgorithm(vector <Tile>& tiles, int index)
{
	vector <int> t = tiles.at(index).getIndexes();
	if (tiles.at(index).Flagged() == true)
	{
		tiles.at(index).setClicked(false);
	}
	else if (tiles.at(index).getNumNeighbors() == 0)
	{
		tiles.at(index).setClicked(true);
		for (unsigned int n = 0; n < t.size(); n++)
		{
			if (tiles.at(t.at(n)).Mine() == false)
			{
				if (tiles.at(t.at(n)).getNumNeighbors() == 0 && tiles.at(t.at(n)).Clicked() == false)
				{
					clickAlgorithm(tiles, t.at(n));
					if (tiles.at(t.at(n)).Flagged() == false)
					{
						tiles.at(t.at(n)).setClicked(true);
					}
				}
				else
				{
					if (tiles.at(t.at(n)).Flagged() == false)
					{
						tiles.at(t.at(n)).setClicked(true);
					}
				}
			}
		}
	}
	else
	{
		tiles.at(index).setClicked(true);
	}
}
void Board::setColumn(int x)
{
	this->columns = x;
}
int Board::getColumn()
{
	return this->columns;
}
void Board::setRow(int x)
{
	this->rows = x;
}
int Board::getRow()
{
	return this->rows;
}

void Board::setMines(int x)
{
	this->bombs = x;
}
int Board::getMines()
{
	return this->bombs;
}
void Board::setFlags(int x)
{
	this->flags = x;
}
int Board::getFlags()
{
	return this->flags;
}

void Board::checkWinner(vector<Tile>& tiles)
{
	bool winner = true;
	for (unsigned int n = 0; n < tiles.size(); n++)
	{
		if (!tiles.at(n).Clicked() && !tiles.at(n).Mine())
		{
			winner = false;
		}
	}
	if (winner == true)
	{
		setStatus("win");
	}
}

