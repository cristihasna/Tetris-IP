#pragma once
#include <SFML\Graphics.hpp>
#include "Pieces.h"

#define ROWS 20
#define COLS 15
#define OFFSET_X 3
#define OFFSET_Y 3
class Board
{
public:

	int board[ROWS][COLS];

	Board(sf::RenderWindow &window);

	~Board();
	void init();
	bool gameOver();
	void clearLine(int &score);
	void merge();
	void Draw(sf::RenderWindow &window, sf::Vector2i nextPiece);


};

