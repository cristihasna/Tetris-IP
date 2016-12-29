#pragma once

#include <SFML\Graphics.hpp>
#include "Pieces.h"
#include "Board.h"
#include <time.h>
class Game
{
public:
	Game(sf::RenderWindow &window);
	~Game();
	void addPieceToBoard(Board &Board, Pieces Pieces);
	void init(Board &Board, Pieces Pieces);
	sf::Vector2i generatePiece(Pieces Pieces);
	bool checkDown(Board &Board);
	bool checkLeft(Board &Board);
	bool checkRight(Board &Board);
	bool checkRotate(Board &Board, Pieces Pieces, sf::Vector2i piece);
	void moveDown(Board &Board);
	void moveRight(Board &Board);
	void moveLeft(Board &Board);
	void Rotate(Board &Board, Pieces Pieces, sf::Vector2i &piece);
	void drawInfo(sf::RenderWindow &window);
	void drawGameOver(sf::RenderWindow &window);
public:
	sf::Vector2i actualPiece;
	sf::Vector2i nextPiece;
	float minutesElapsed, secondsElapsed, delay;
	int score, speed;
};

