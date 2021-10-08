#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "Tile.h"

using namespace std;

Tile::Tile()
{
	this->x = 0;
	this->y = 0;
	this->isFlagged = false;
	this->isMine = false;
	this->isClicked = false;
}

Tile::Tile(float x, float y)
{
	this->x = x;
	this->y = y;
	this->isFlagged = false;
	this->isMine = false;
	this->isClicked = false;
}

float Tile::getX()
{
	return this->x;
}

void Tile::setX(float x)
{
	this->x = x;
}

float Tile::getY()
{
	return this->y;
} 

void Tile::setY(float y)
{
	this->y = y;
}

bool Tile::Mine()
{
	if (this->isMine)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Tile::setMine(bool b)
{
	this->isMine = b;
}

void Tile::setFlagged(bool b)
{
	this->isFlagged = b;
}

bool Tile::Flagged()
{
	if (this->isFlagged)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Tile::setClicked(bool b)
{
	this->isClicked = b;
}

bool Tile::Clicked()
{
	if (this->isClicked)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Tile::setNumNeighbors(int n)
{
	this->numNeighbors = n;
}

int Tile::getNumNeighbors()
{
	return this->numNeighbors;
}
vector<int> Tile::getNeighborIndex(vector<Tile>& tiles, int index, int columns, int rows)
{
	int count = 0;
	int adj1 = index - (columns+1); //upper left
	int adj2 = index - columns; //above
	int adj3 = index - (columns-1); //upper right
	int adj4 = index - 1; //left
	int adj5 = index + 1; //right
	int adj6 = index + (columns-1); //lower left
	int adj7 = index + columns; //below
	int adj8 = index + (columns+1); //lower right

	if (index == 0) //top left
	{
		this->neighborIndexes.push_back(adj5);
		this->neighborIndexes.push_back(adj7);
		this->neighborIndexes.push_back(adj8);
	}
	else if (index == columns - 1) //top right
	{
		this->neighborIndexes.push_back(adj4);
		this->neighborIndexes.push_back(adj6);
		this->neighborIndexes.push_back(adj7);
	}
	else if (index == (rows * columns) - columns) //bottom left
	{
		this->neighborIndexes.push_back(adj2);
		this->neighborIndexes.push_back(adj3);
		this->neighborIndexes.push_back(adj5);
	}
	else if (index == (rows * columns) - 1) //bottom right
	{
		this->neighborIndexes.push_back(adj1);
		this->neighborIndexes.push_back(adj2);
		this->neighborIndexes.push_back(adj4);
	}
	else if (index < columns) //top edge
	{
		this->neighborIndexes.push_back(adj4);
		this->neighborIndexes.push_back(adj5);
		this->neighborIndexes.push_back(adj6);
		this->neighborIndexes.push_back(adj7);
		this->neighborIndexes.push_back(adj8);
	}
	else if (index > (columns * (rows - 1))) //bottom edge
	{
		this->neighborIndexes.push_back(adj1);
		this->neighborIndexes.push_back(adj2);
		this->neighborIndexes.push_back(adj3);
		this->neighborIndexes.push_back(adj4);
		this->neighborIndexes.push_back(adj5);
	}
	else if ((index + columns) % columns == 0)//left edge
	{
		this->neighborIndexes.push_back(adj2);
		this->neighborIndexes.push_back(adj3);
		this->neighborIndexes.push_back(adj5);
		this->neighborIndexes.push_back(adj7);
		this->neighborIndexes.push_back(adj8);
	}
	else if ((index + 1) % columns == 0)//right edge
	{
		this->neighborIndexes.push_back(adj1);
		this->neighborIndexes.push_back(adj2);
		this->neighborIndexes.push_back(adj4);
		this->neighborIndexes.push_back(adj6);
		this->neighborIndexes.push_back(adj7);
	}
	else
	{
		this->neighborIndexes.push_back(adj1);
		this->neighborIndexes.push_back(adj2);
		this->neighborIndexes.push_back(adj3);
		this->neighborIndexes.push_back(adj4);
		this->neighborIndexes.push_back(adj5);
		this->neighborIndexes.push_back(adj6);
		this->neighborIndexes.push_back(adj7);
		this->neighborIndexes.push_back(adj8);
	}

	return this->neighborIndexes;
}

vector<int> Tile::getIndexes()
{
	return this->neighborIndexes;
}

