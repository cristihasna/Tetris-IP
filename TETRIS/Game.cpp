#include "Game.h"
#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>

Game::Game(sf::RenderWindow &window)
{
}


Game::~Game()
{
}
void Game::addPieceToBoard(Board &Board, char pieces[7][4][5][5]) {

	if (!isPieceHold) {
		currentPiece = nextPiece;
		nextPiece = Game::generatePiece();
	}

	sf::Vector2i initialPos;
	initialPos.x = initialPos.y = -1;

	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			if (initialPos.x == -1 && pieces[currentPiece.x][currentPiece.y][i][j] != 0)
				initialPos.x = i;

	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			if (initialPos.y == -1 && pieces[currentPiece.x][currentPiece.y][j][i] != 0)
				initialPos.y = i;


	for (int i = 0; i < 5 - initialPos.x; i++) {
		for (int j = 0, j2 = (COLS - 5) / 2 + initialPos.y; j < 5 - initialPos.y; j++, j2++) {
			if ((pieces[currentPiece.x][currentPiece.y][i + initialPos.x][j + initialPos.y] == 1 || pieces[currentPiece.x][currentPiece.y][i + initialPos.x][j + initialPos.y] == 2) && Board.board[i][j2].value == 0) {
				Board.board[i][j2].value = pieces[currentPiece.x][currentPiece.y][i + initialPos.x][j + initialPos.y];
				Board.board[i][j2].color = currentPiece.z;
			}
		}
	}
}
void Game::init(Board &Board, char pieces[7][4][5][5]) {
	srand(time(NULL));
	Board.init();
	nextPiece = generatePiece();

	heldPiece.x = -1;
	heldPiece.y = -1;
	heldPiece.z = -1;

	isPieceHold = false;

	addPieceToBoard(Board, pieces);
	minutesElapsed = 0;
	secondsElapsed = 0;
	score = 0;
	delay = 0.7;
	powerUpActiveTime = 10;
	powerUpTimer = 0;
	powerUpVisibleTime = 10;
	collectedPU = 0;
	generatePowerUpDelay = 15;
	generatePowerUp(Board, generatePowerUpDelay);


}
sf::Vector3i Game::generatePiece() {
	int piece = rand() % 7;
	int rotation = rand() % 4;
	int color = rand() % 4;
	sf::Vector3i result;
	result.x = piece;
	result.y = rotation;
	result.z = color;
	return result;
}

void Game::holdPiece(Board &Board, char pieces[7][4][5][5]) {
	if (heldPiece.x == -1) {
		heldPiece = currentPiece;
		currentPiece = nextPiece;
		nextPiece = generatePiece();
	}
	else {
		sf::Vector3i aux = currentPiece;
		currentPiece = heldPiece;
		heldPiece = aux;
	}
	for(int i=0;i<ROWS;i++)
		for(int j=0;j<COLS;j++)
			if (Board.board[i][j].value == 1 || Board.board[i][j].value == 2) {
				Board.board[i][j].value = 0;
				Board.board[i][j].color = 0;
			}
	isPieceHold = true;
	addPieceToBoard(Board, pieces);

}
bool Game::checkDown(Board &Board) {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			if (Board.board[i][j].value == 1 || Board.board[i][j].value == 2) {
				if (i == ROWS - 1) {
					return false;
				}
				else if (Board.board[i + 1][j].value == 3) {
					return false;
				}
			}
		}
	}
	return true;
}
bool Game::checkLeft(Board &Board) {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			if (Board.board[i][j].value == 1 || Board.board[i][j].value == 2) {
				if (j == 0) {
					return false;
				}
				else if (Board.board[i][j - 1].value == 3) {
					return false;
				}
			}
		}
	}
	return true;
}
bool Game::checkRight(Board &Board) {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			if (Board.board[i][j].value == 1 || Board.board[i][j].value == 2) {
				if (j == COLS - 1) {
					return false;
				}
				else if (Board.board[i][j + 1].value == 3) {
					return false;
				}
			}
		}
	}
	return true;
}
bool Game::checkRotate(Board &Board, sf::Vector3i piece, char pieces[7][4][5][5]) {
	int pivotX = 0, pivotY = 0;
	for (int i = 0; i < ROWS && (pivotX == 0 && pivotY == 0); i++) {
		for (int j = 0; j<COLS && (pivotX == 0 && pivotY == 0); j++)
			if (Board.board[i][j].value == 2) {
				pivotX = j;
				pivotY = i;
			}
	}
	if (pivotY <= 1) return false;
	int nextRotation = (piece.y + 1) % 4;
	for (int i = pivotY - 2, iP = 0; i <= pivotY + 2; i++, iP++)
		for (int j = pivotX - 2, jP = 0; j <= pivotX + 2; j++, jP++)
			if ((i >= ROWS || j >= COLS || j <0 || i<0 || Board.board[i][j].value == 3) && (pieces[piece.x][nextRotation][iP][jP] == 1 || pieces[piece.x][nextRotation][iP][jP] == 2))
				return false;
	return true;
}

void Game::moveDown(Board &Board) {
	for (int i = ROWS - 1; i >= 0; i--)
		for (int j = 0; j < COLS; j++)
			if (Board.board[i - 1][j].value == 1 || Board.board[i - 1][j].value == 2 || Board.board[i][j].value == 1 || Board.board[i][j].value == 2)
			{
				if (Board.board[i][j].value == 4 || Board.board[i][j].value == 5 || Board.board[i][j].value == 6 || Board.board[i][j].value==7) {
					collectedPU = Board.board[i][j].value;
					powerUpTimer = 0;
					if (collectedPU == 5) {
						clearColorPU(Board, currentPiece.z, score);
						collectedPU = 0;
					}
					else if (collectedPU == 7) {
						score += 250;
						collectedPU = 0;
					}
				}
				if (i == 0 || Board.board[i - 1][j].value == 3 || Board.board[i - 1][j].value == 4 || Board.board[i - 1][j].value == 5 || Board.board[i - 1][j].value == 6 || Board.board[i-1][j].value == 7) {

					Board.board[i][j].value = 0;
					Board.board[i][j].color = 0;
				}
				else Board.board[i][j] = Board.board[i - 1][j];
			}
}
void Game::moveRight(Board &Board) {
	for (int i = 0; i <ROWS; i++)
		for (int j = COLS - 1; j > 0; j--)
			if (Board.board[i][j - 1].value == 1 || Board.board[i][j - 1].value == 2 || Board.board[i][j].value == 1 || Board.board[i][j].value == 2)
			{
				if (Board.board[i][j].value == 4 || Board.board[i][j].value == 5 || Board.board[i][j].value == 6 || Board.board[i][j].value == 7) {
					collectedPU = Board.board[i][j].value;
					powerUpTimer = 0;
					if (collectedPU == 5) {
						clearColorPU(Board, currentPiece.z, score);
						collectedPU = 0;
					}
					else if (collectedPU == 7) {
						score += 250;
						collectedPU = 0;
					}
				}
				if (Board.board[i][j - 1].value == 3 || Board.board[i][j - 1].value == 4 || Board.board[i][j - 1].value == 5 || Board.board[i][j - 1].value == 6 || Board.board[i][j-1].value == 7) {
					Board.board[i][j].value = 0;
					Board.board[i][j].color = 0;
				}
				else{
					Board.board[i][j] = Board.board[i][j - 1];
				}
			}
	for (int i = 0; i < ROWS; i++)
		if (Board.board[i][0].value == 1 || Board.board[i][0].value == 2) {
			Board.board[i][0].value = 0;
			Board.board[i][0].color = 0;
		}
}
void Game::moveLeft(Board &Board) {
	for (int i = 0; i <ROWS; i++)
		for (int j = 0; j < COLS; j++)
			if (Board.board[i][j + 1].value == 1 || Board.board[i][j + 1].value == 2 || Board.board[i][j].value == 1 || Board.board[i][j].value == 2)
			{
				if (Board.board[i][j].value == 4 || Board.board[i][j].value == 5 || Board.board[i][j].value == 6 || Board.board[i][j].value == 7) {
					collectedPU = Board.board[i][j].value;
					powerUpTimer = 0;
					if (collectedPU == 5) {
						clearColorPU(Board, currentPiece.z, score);
						collectedPU = 0;
					}
					else if (collectedPU == 7) {
						score += 250;
						collectedPU = 0;
					}
				}
				if (j == COLS - 1 || Board.board[i][j + 1].value == 3 || Board.board[i][j + 1].value == 4 || Board.board[i][j + 1].value == 5 || Board.board[i][j + 1].value == 6 || Board.board[i][j+1].value == 7) {
					Board.board[i][j].value = 0;
					Board.board[i][j].color = 0;
				}
				else Board.board[i][j] = Board.board[i][j + 1];
			}
}
void Game::Rotate(Board &Board,  sf::Vector3i &piece, char pieces[7][4][5][5]) {
	int pivotX = 0, pivotY = 0;
	for (int i = 0; i < ROWS && (pivotX == 0 && pivotY == 0); i++) {
		for (int j = 0; j<COLS && (pivotX == 0 && pivotY == 0); j++)
			if (Board.board[i][j].value == 2) {
				pivotX = j;
				pivotY = i;
			}
	}
	piece.y = (piece.y + 1) % 4;
	for (int i = pivotY - 2, iP = 0; i <= pivotY + 2; i++, iP++)
		for (int j = pivotX - 2, jP = 0; j <= pivotX + 2; j++, jP++)
		{
			if (Board.board[i][j].value != 3 && Board.board[i][j].value != 4 && Board.board[i][j].value != 5 && Board.board[i][j].value!=6 && Board.board[i][j].value!=7) {
				Board.board[i][j].value = pieces[piece.x][piece.y][iP][jP];
				Board.board[i][j].color = piece.z;
			}
			else if ((Board.board[i][j].value == 4 || Board.board[i][j].value == 5 || Board.board[i][j].value == 6 || Board.board[i][j].value == 7) && (pieces[piece.x][piece.y][iP][jP] == 1 || pieces[piece.x][piece.y][iP][jP] == 2)) {
				collectedPU = Board.board[i][j].value;

				powerUpTimer = 0;
				Board.board[i][j].value = pieces[piece.x][piece.y][iP][jP];
				Board.board[i][j].color = piece.z;
				if (collectedPU == 5) {
					clearColorPU(Board, piece.z, score);
					collectedPU = 0;
				}
				else if (collectedPU == 7) {
					score += 250;
					collectedPU = 0;
				}
					
			}
		}
}
void Game::drawInfo(sf::RenderWindow &window, HighScores HighScores) {
	speed = 100 - delay * 100;
	sf::Font gameInfoFont;
	gameInfoFont.loadFromFile("fonts/Roboto-Medium.ttf");
	sf::Text gameInfo[5];
	std::string gameScore = std::to_string(score);
	std::string minutes = std::to_string((int)minutesElapsed);
	if (minutesElapsed <= 9)
		minutes = "0" + minutes;
	std::string seconds = std::to_string((int)secondsElapsed);
	if (secondsElapsed <= 9)
		seconds = "0" + seconds;
	std::string gameTime = minutes + ':' + seconds;
	std::string speedString = std::to_string(speed);

	gameInfo[0].setString(gameScore);
	gameInfo[0].setPosition(sf::Vector2f((OFFSET_X + COLS + 1) * 18 + 3, (OFFSET_Y + 6) * 18));
	gameInfo[0].setFillColor(sf::Color::White);
	gameInfo[0].setFont(gameInfoFont);
	gameInfo[0].setCharacterSize(24);

	gameInfo[1].setString(gameTime);
	gameInfo[1].setPosition(sf::Vector2f((OFFSET_X + COLS + 1) * 18 + 3, (OFFSET_Y + 8) * 18 + 56));
	gameInfo[1].setFillColor(sf::Color::White);
	gameInfo[1].setFont(gameInfoFont);
	gameInfo[1].setCharacterSize(24);

	gameInfo[2].setString(speedString);
	gameInfo[2].setPosition(sf::Vector2f((OFFSET_X + COLS + 1) * 18 + 3, (OFFSET_Y + 7) * 18 + 28));
	gameInfo[2].setFillColor(sf::Color::White);
	gameInfo[2].setFont(gameInfoFont);
	gameInfo[2].setCharacterSize(24);

	Score scoreToBeat = HighScores.getScoreToBeat(Game::score);
	gameInfo[3].setString(std::to_string(scoreToBeat.score));
	gameInfo[3].setPosition(sf::Vector2f((OFFSET_X + COLS + 1) * 18 + 3, (OFFSET_Y + 9) * 18 + 84));
	gameInfo[3].setFillColor(sf::Color::White);
	gameInfo[3].setFont(gameInfoFont);
	gameInfo[3].setCharacterSize(24);

	if (scoreToBeat.score == score) {
		scoreToBeat.minutesElapsed = minutesElapsed;
		scoreToBeat.secondsElapsed = secondsElapsed;
	}
	minutes = std::to_string((int)scoreToBeat.minutesElapsed);
	if (scoreToBeat.minutesElapsed <= 9)
		minutes = "0" + minutes;
	seconds = std::to_string((int)scoreToBeat.secondsElapsed);
	if (scoreToBeat.secondsElapsed <= 9)
		seconds = "0" + seconds;

	gameTime = minutes + ":" + seconds;

	gameInfo[4].setString(gameTime);
	gameInfo[4].setPosition(sf::Vector2f((OFFSET_X + COLS + 1) * 18 + 3, (OFFSET_Y + 10) * 18+95));
	gameInfo[4].setFillColor(sf::Color::White);
	gameInfo[4].setFont(gameInfoFont);
	gameInfo[4].setCharacterSize(13);
	for (int i = 0; i<5; i++)
		window.draw(gameInfo[i]);

	if (collectedPU == 4 || collectedPU==6) {
		sf::Texture activePowerUpTexture, shadowBg;
		activePowerUpTexture.loadFromFile("images/tiles.png");
		shadowBg.loadFromFile("images/shadow.png");
		sf::Sprite activePowerUp(activePowerUpTexture);
		sf::Sprite shadow(shadowBg);
		shadow.setTextureRect(sf::IntRect(0, 0, 146, 36));
		shadow.setPosition(sf::Vector2f((OFFSET_X + (COLS) / 2) * 18-64, (OFFSET_Y + ROWS + 1) * 18-9));
		activePowerUp.setTextureRect(sf::IntRect(Game::collectedPU % 4 * 18, 0, 18, 18));
		activePowerUp.setPosition(sf::Vector2f((OFFSET_X + (COLS)/2) * 18, (OFFSET_Y + ROWS + 1) * 18));
		window.draw(activePowerUp);
		window.draw(shadow);
	}
}
void Game::drawGameOver(sf::RenderWindow &window) {
	sf::Texture goBackground;
	goBackground.loadFromFile("images/gameOver.png");
	sf::Sprite goSprite(goBackground);
	goSprite.setTextureRect(sf::IntRect(0, 0, 720, 480));
	window.draw(goSprite);
}
void Game::generatePowerUp(Board &Board, float &generatePowerUpDelay) {

	int powerUps[4] = { 4 , 5, 6, 7};
	int delays[5] = { 10, 5, 15, 3, 7 };
	generatePowerUpDelay = delays[rand() % 5];

	int puX, puY;
	do {
		puX = rand() % COLS;
		puY = rand() % (ROWS-4)+4;
	} while (Board.board[puY][puX].value != 0);
	int powerUpFrequency = rand() % 150;
	int puIndex;
	if (Board.firstRow <= 12)
	{
		if (powerUpFrequency <= 50)
			puIndex = 1;

		else if (powerUpFrequency <= 90)
			puIndex = 3;

		else if (powerUpFrequency <= 120)
			puIndex = 0;

		else puIndex = 2;

	}
	else {
		if (powerUpFrequency <= 20)
			puIndex = 1;

		else if (powerUpFrequency <= 50)
			puIndex = 3;

		else if (powerUpFrequency <= 100)
			puIndex = 0;

		else puIndex = 2;
	}
	Board.board[puY][puX].value = powerUps[puIndex];
}
void Game::clearColorPU(Board &Board, int color, int &score) {
	for (int i = ROWS - 1; i > 0; i--) {
		for (int j = 0; j < COLS; j++) {
			if (Board.board[i][j].color == color && Board.board[i][j].value==3) {
				score += 10;
				for (int p = i; p > 0; p--)
				{
					if (Board.board[p - 1][j].value != 1 && Board.board[p-1][j].value!=2)
						Board.board[p][j] = Board.board[p - 1][j];
					else {
						Board.board[p][j].value = 0;
						Board.board[p][j].color = 0;
						break;
					}
						
				}
					
				j--;
			}
		}
	}
}