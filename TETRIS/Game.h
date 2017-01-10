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
	sf::Vector3i generatePiece(Pieces Pieces);
	bool checkDown(Board &Board);
	bool checkLeft(Board &Board);
	bool checkRight(Board &Board);
	bool checkRotate(Board &Board, Pieces Pieces, sf::Vector3i piece);
	void moveDown(Board &Board);
	void moveRight(Board &Board);
	void moveLeft(Board &Board);
	void Rotate(Board &Board, Pieces Pieces, sf::Vector3i &piece);
	void drawInfo(sf::RenderWindow &window);
	void drawGameOver(sf::RenderWindow &window);
	void generatePowerUp(Board &Board, Pieces Pieces);

public:
	sf::Vector3i actualPiece;
	sf::Vector3i nextPiece;
	float minutesElapsed, secondsElapsed, delay, powerUpActiveTime, powerUpTimer;
	int score, speed;
	char collectedPU = 0;
};

