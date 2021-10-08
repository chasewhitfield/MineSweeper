#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Tile.h"
#include "Board.h"
#include <chrono>
#include <time.h>
#include<fstream>
#include<sstream>
using namespace std;

int main()
{
    int column, row, mine;
    vector<int> mines; //variables only used within main method 
    void readFile(int& column, int& row, int& mine); //read .cfg file to set column, row, and mine variables
    void readTest(string test, vector<Tile> &tiles, Board &b); //read in test file and update each tile
    void setMineIndex(int column, int row, int mine, vector<int> &mines); //randomize and reallocate mines when game reset
    string getNumPNG(int n); //imports png path for proper number of adjacent tiles
    vector<int> getDigitPNG(int n); //display mines - flags

    readFile(column, row, mine);
    setMineIndex(column, row, mine, mines); 

    Board b(column, row); //singular board object created


    b.setColumn(column);
    b.setRow(row);
    b.setMines(mine); //set board conditions from config file

    sf::Texture debugText;
    debugText.loadFromFile("images/debug.png");
    sf::Sprite debug;
    debug.setTexture(debugText);
    debug.setPosition(464, b.getRow() * 32); //make debug button

    sf::Texture happyText;
    happyText.loadFromFile("images/face_happy.png");
    sf::Sprite happy;
    happy.setTexture(happyText);
    happy.setPosition(330, b.getRow() * 32); //make 'playing' condition

    sf::Texture loseText;
    loseText.loadFromFile("images/face_lose.png");
    sf::Sprite sad;
    sad.setTexture(loseText);
    sad.setPosition(330, b.getRow() * 32); //make 'lose' condition

    sf::Texture winText;
    winText.loadFromFile("images/face_win.png");
    sf::Sprite win;
    win.setTexture(winText);
    win.setPosition(330, b.getRow() * 32); //make 'win' condition

    sf::Texture oneText;
    oneText.loadFromFile("images/test_1.png");
    sf::Sprite testOne;
    testOne.setTexture(oneText);
    testOne.setPosition(524, b.getRow() * 32); //make test 1 button

    sf::Texture twoText;
    twoText.loadFromFile("images/test_2.png");
    sf::Sprite testTwo;
    testTwo.setTexture(twoText);
    testTwo.setPosition(584, b.getRow() * 32); //make test 2 button

    sf::Texture threeText;
    threeText.loadFromFile("images/test_3.png");
    sf::Sprite testThree;
    testThree.setTexture(threeText);
    testThree.setPosition(644, b.getRow() * 32); //make test 3 button

    sf::RenderWindow window(sf::VideoMode(b.getColumn()*32, b.getRow() *32 + 100), "Minesweeper"); //create window

    vector<Tile> tiles; //establish tiles vector

    for (unsigned int n = 0; n < b.getRow(); n++)
    {
        for (unsigned int m = 0; m < b.getColumn(); m++)
        {
            Tile t = Tile((float)(m * 32), (float)(n * 32)); //set x,y values in window and make tile
            int raw = m + b.getColumn() * n; //index in tiles vector
            tiles.push_back(t);
        }
    }

    for (unsigned int n = 0; n < tiles.size(); n++)
    {
        for (unsigned int m = 0; m < mines.size(); m++)
        {
            if (n == mines.at(m))
            {
                tiles.at(n).setMine(true); //find indexes that match with mine numbers and set them
            }
        }
    }

    for (unsigned int n = 0; n < tiles.size(); n++)
    {
        tiles.at(n).getNeighborIndex(tiles, n, b.getColumn(),b.getRow()); //find what tiles to look at for each tile
        b.findNumNeigbors(tiles, n, b.getColumn(), b.getRow()); //from those tiles, find number of mines
    }
    sf::Texture digitText;
    sf::Sprite digits;
    while (window.isOpen())
    {
        vector<int> digs = getDigitPNG(b.getMines() - b.getFlags()); //get digits in order 
        for (unsigned int n = 0; n < digs.size(); n++)
        {
            digitText.loadFromFile("images/digits.png", sf::IntRect((digs.at(n)*21), 0, 21, 32));
            digits.setTexture(digitText);
            digits.setPosition((n*21), row * 32);
            window.draw(digits); //print each digit as the counter 
        }
        window.draw(debug);
        window.draw(testOne);
        window.draw(testTwo);
        window.draw(testThree); //draw bottom console
        if (b.showStatus() == "win") //draw console's face based on game's status
        {
            window.draw(win);
        }
        else if (b.showStatus() == "lose")
        {
            window.draw(sad);
        }
        else
        {
            window.draw(happy);
        }

        sf::Texture texture;
        sf::Sprite sprite;
        for (unsigned int n = 0; n < tiles.size(); n++)
        {
            if (tiles.at(n).Clicked()) //if a tile is clicked, 
            {
                texture.loadFromFile("images/tile_revealed.png"); //reveal it
                sprite.setTexture(texture);
                sprite.setPosition(tiles.at(n).getX(), tiles.at(n).getY()); //change position each iteration
                window.draw(sprite); 
                if (tiles.at(n).Mine()) //if tile is a mine
                {
                    if (tiles.at(n).Flagged()) // and flagged
                    {
                        texture.loadFromFile("images/flag.png"); //draw the flag underneath
                        sprite.setTexture(texture);
                        window.draw(sprite);
                    }
                    texture.loadFromFile("images/mine.png"); //draw mine if it's a mine
                    sprite.setTexture(texture);
                    window.draw(sprite);
                }
                else
                {
                    int num = tiles.at(n).getNumNeighbors(); //find number of adjacent mines
                    if (num != 0) //if it has neighboring mines
                    {
                        texture.loadFromFile(getNumPNG(num));
                        sprite.setTexture(texture);
                        window.draw(sprite); //show num neighbors
                    }
                }
            }
            else
            {
                texture.loadFromFile("images/tile_hidden.png"); //no click? keep it hidden
                sprite.setTexture(texture);
                sprite.setPosition(tiles.at(n).getX(), tiles.at(n).getY());
                window.draw(sprite);
                if (tiles.at(n).Flagged())
                {
                    texture.loadFromFile("images/flag.png"); //print a flag over a hidden space if it was flagged
                    sprite.setTexture(texture);
                    sprite.setPosition(tiles.at(n).getX(), tiles.at(n).getY());
                    window.draw(sprite);
                }
            }
            if (b.getDebug() == true) //if in debug mode
            {
                if (tiles.at(n).Mine() && !(b.showStatus() == "true"))
                {
                    texture.loadFromFile("images/mine.png"); //show all mines (no need to have them clicked)
                    sprite.setTexture(texture);
                    window.draw(sprite);
                }
            }
        }
        window.display();
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) //if left click
            {
                int x = sf::Mouse::getPosition(window).x / 32;
                int y = sf::Mouse::getPosition(window).y / 32;
                int index = x + b.getColumn() * y; //get index from mouse position
                b.setMines(mine); //set number of mines as originally read in
                if (index > b.getRow()*b.getColumn()) //if not clicked on any tiles
                {
                    if (sf::Mouse::getPosition(window).x >= 330 && sf::Mouse::getPosition(window).x < 390) //if over face
                    {
                        setMineIndex(b.getColumn(), b.getRow(), b.getMines(), mines); //generate new random mine
                        int bomb = 0;
                        for (unsigned int n = 0; n < tiles.size(); n++) //reset values to be false and place mines as needed
                        {
                            tiles.at(n).setClicked(false);
                            tiles.at(n).setFlagged(false);
                            tiles.at(n).setMine(false); //set everything back to unclicked and have no mines
                            for (unsigned int m = 0; m < mines.size(); m++)
                            {
                                if (n == mines.at(m))
                                {
                                    tiles.at(n).setMine(true); //set mines as randomly assigned
                                    bomb++; //add to bomb count
                                }
                            }
                        }
                        for (unsigned int n = 0; n < tiles.size(); n++) //recalculate neighboring mines
                        {
                            b.findNumNeigbors(tiles, n, column, row); 
                        }
                        b.setStatus("game");//reset game
                        b.setFlags(0); //reset so no flags
                        b.setMines(bomb); //set mine number again
                    }
                    else if (sf::Mouse::getPosition(window).x >= 464 && sf::Mouse::getPosition(window).x < 524 && (b.showStatus() != "win" || b.showStatus() != "lose")) //if debug clicked and game still running
                    {
                        //toggle debug value
                        if (b.getDebug() == false)
                        {
                            b.setDebug(true);
                        }
                        else
                        {
                            b.setDebug(false);
                        }
                    }
                    else if (sf::Mouse::getPosition(window).x >= 524 && sf::Mouse::getPosition(window).x < 584) //if test board 1, load it and its assigned contents. Same for 2 and 3 below
                    {
                        readTest("boards/testboard1.brd", tiles, b);
                    }
                    else if (sf::Mouse::getPosition(window).x >= 584 && sf::Mouse::getPosition(window).x < 644)
                    {
                        readTest("boards/testboard2.brd", tiles, b);
                    }
                    else if (sf::Mouse::getPosition(window).x >= 644 && sf::Mouse::getPosition(window).x < 704)
                    {
                        readTest("boards/testboard3.brd", tiles, b);
                    }
                }
                else if(!(b.showStatus() == "win" || b.showStatus() == "lose")) //if we have NOT won the game
                {
                    if (tiles.at(index).Mine() && tiles.at(index).Flagged() == false) //if not flagged and mine clicked on, show all mines and end game
                    {
                        b.showMines(tiles); //show every mine
                        b.setStatus("lose"); //set status to lose to change face
                    }
                    else //otherwise go through click algorithm
                    {
                        b.clickAlgorithm(tiles, index);
                    }
                }
                b.checkWinner(tiles); //after a click check if game is won
                if (b.showStatus() == "win" && (b.getMines() - b.getFlags()) > 0)
                {
                    for (unsigned int n = 0; n < tiles.size(); n++)
                    {
                        if (tiles.at(n).Mine())
                        {
                            tiles.at(n).setFlagged(true); //set the mines to be flagged while still unrevealed
                            b.setFlags(b.getFlags() + 1); //add one to flag so counter accurate
                        }
                    }
                    b.setDebug(false); //turn debug off so mines don't show
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right && !(b.showStatus() == "win" || b.showStatus() == "lose")) //if right click while game in progress
            {
                int x = sf::Mouse::getPosition(window).x / 32;  
                int y = sf::Mouse::getPosition(window).y / 32;
                int index = x + column * y;
                if (tiles.at(index).Flagged())
                {
                    tiles.at(index).setFlagged(false); //unflag if flagged
                    b.setFlags(b.getFlags() - 1); //remove one from flag count
                }
                else
                {
                    tiles.at(index).setFlagged(true); //set flag if unflagged
                    b.setFlags(b.getFlags() + 1); //add to flag count
                }
            }
        }
        window.clear();
    }

    return 0;
}

string getNumPNG(int a) //all methods described at top
{
    if (a == 1)
    {
        return "images/number_1.png";
    }
    else if (a == 2)
    {
        return "images/number_2.png";
    }
    else if (a == 3)
    {
        return "images/number_3.png";
    }
    else if (a == 4)
    {
        return "images/number_4.png";
    }
    else if (a == 5)
    {
        return "images/number_5.png";
    }
    else if (a == 6)
    {
        return "images/number_6.png";
    }
    else if (a == 7)
    {
        return "images/number_7.png";
    }
    else if (a == 8)
    {
        return "images/number_8.png";
    }
}

void readFile(int& column, int& row, int& mine)
{
    ifstream inFile("boards/config.cfg");
    string line;
    vector<string> lines;
    if (inFile.is_open())
    {
        while (getline(inFile, line))
        {
            lines.push_back(line);
        }
    }

    column = stoi(lines.at(0));
    row = stoi(lines.at(1));
    mine = stoi(lines.at(2)); //all values set as needed by .cfg
}

void setMineIndex(int column, int row, int mine, vector<int> &mines)
{
    int num = 0;
    mines.clear(); //clear mine index so we don't double number of mines!
    srand(time(NULL)); 
    while (num < mine)
    {
        int temp = rand() % (column * row); //set random value
        bool duplicate = false; //checks that we don't have a repeat position for a mine
        if (mines.size() == 0)
        {
            mines.push_back(temp);
            num++;
        }
        else
        {
            for (unsigned int n = 0; n < mines.size(); n++)
            {
                if (temp == mines.at(n))
                {
                    duplicate = true; //check every single mine already set to ensure no repeat
                }
            }
            if (duplicate == false) //not a duplicate? add it on!
            {
                mines.push_back(temp);
                num++;
            }
        }
    }
}

void readTest(string test, vector<Tile>& tiles, Board &b)
{
    ifstream inFile(test);
    string line;
    vector<string> lines;
    if (inFile.is_open())
    {
        while (getline(inFile, line))
        {
            lines.push_back(line);
        }
    }
    vector<int> values;
    for (unsigned int n = 0; n < lines.size(); n++)
    {
        for (unsigned int m = 0; m < lines.at(n).length(); m++)
        {
            int i = stoi(lines.at(n).substr(m, 1));
            values.push_back(i);
        }
    }
    int mines = 0;
    for (unsigned int n = 0; n < tiles.size(); n++)
    {
        tiles.at(n).setClicked(false);
        tiles.at(n).setFlagged(false);
        tiles.at(n).setMine(false);
        
        if (values.at(n) == 1) //one data read in, if an index has a 1 then put a mine there
        {
            tiles.at(n).setMine(true);
            mines++;
        }
    }
    b.setFlags(0); //ensure no flags
    b.setMines(mines); //set mines as read in
    
    for (unsigned int n = 0; n < tiles.size(); n++)
    {
        b.findNumNeigbors(tiles, n, b.getColumn(), b.getRow()); //recalibrate number of neighbors
    }
    b.setStatus("game"); //ensure game is in progress
}

vector<int> getDigitPNG(int a)
{
    vector<int> x;
    if (a < 0)
    {
        x.push_back(10); //if negative, get a negative in first term
        a *= -1;
    }
    int num1 = a % 10; //get modulos to find how to rep number on counter
    a /= 10;
    int num2 = a % 10;
    a /= 10;
    int num3 = a % 10;
    a /= 10;
    x.push_back(num3);
    x.push_back(num2);
    x.push_back(num1);
    return x;
}
