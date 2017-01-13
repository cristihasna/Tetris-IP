#pragma once

#include <SFML\Graphics.hpp>
#include "Board.h"
#include "HighScores.h"
#include <time.h>
class Game
{
public:
	Game(sf::RenderWindow &window);
	~Game();
	void addPieceToBoard(Board &Board, char pieces[7][4][5][5]);
	void init(Board &Board, char pieces[7][4][5][5]);
	sf::Vector3i generatePiece();
	bool checkDown(Board &Board);
	bool checkLeft(Board &Board);
	bool checkRight(Board &Board);
	bool checkRotate(Board &Board, sf::Vector3i piece, char pieces[7][4][5][5]);
	void moveDown(Board &Board);
	void moveRight(Board &Board);
	void moveLeft(Board &Board);
	void Rotate(Board &Board, sf::Vector3i &piece, char pieces[7][4][5][5]);
	void drawInfo(sf::RenderWindow &window, HighScores HighScores);
	void drawGameOver(sf::RenderWindow &window);
	void generatePowerUp(Board &Boards, float &generatePowerUpDelay);
	void clearColorPU(Board &Board, int color, int &score);
	void holdPiece(Board &Board, char pieces[7][4][5][5]);

public:
	sf::Vector3i currentPiece;
	sf::Vector3i nextPiece;
	sf::Vector3i heldPiece;
	float minutesElapsed, secondsElapsed, delay, powerUpActiveTime, powerUpTimer, generatePowerUpDelay, powerUpVisibleTime;
	int score, speed;
	char collectedPU = 0;
	bool isPieceHold;
};

