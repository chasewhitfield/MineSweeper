#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace std;


class Tile
{
private:
	int x, y, numNeighbors;
	bool isFlagged, isMine, isClicked;
	vector <int> neighborIndexes;
public:
	Tile();
	Tile(float x, float y);
	float getX();
	float getY();
	void setX(float a);
	void setY(float a);
	void setMine(bool b);
	bool Mine();
	void setFlagged(bool b);
	bool Flagged();
	void setClicked(bool b);
	bool Clicked();
	void setNumNeighbors(int n);
	int getNumNeighbors();
	vector<int> getNeighborIndex(vector<Tile>& tiles, int index, int columns, int rows);
	vector<int> getIndexes();
};
