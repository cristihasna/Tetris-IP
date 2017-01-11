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

	actualPiece = nextPiece;
	nextPiece = Game::generatePiece();

	sf::Vector2i initialPos;
	initialPos.x = initialPos.y = -1;

	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			if (initialPos.x == -1 && pieces[actualPiece.x][actualPiece.y][i][j] != 0)
				initialPos.x = i;

	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			if (initialPos.y == -1 && pieces[actualPiece.x][actualPiece.y][j][i] != 0)
				initialPos.y = i;


	for (int i = 0; i < 5 - initialPos.x; i++) {
		for (int j = 0, j2 = (COLS - 5) / 2 + initialPos.y; j < 5 - initialPos.y; j++, j2++) {
			if ((pieces[actualPiece.x][actualPiece.y][i + initialPos.x][j + initialPos.y] == 1 || pieces[actualPiece.x][actualPiece.y][i + initialPos.x][j + initialPos.y] == 2) && Board.board[i][j2].value == 0) {
				Board.board[i][j2].value = pieces[actualPiece.x][actualPiece.y][i + initialPos.x][j + initialPos.y];
				Board.board[i][j2].color = actualPiece.z;
			}
		}
	}
}
void Game::init(Board &Board, char pieces[7][4][5][5]) {
	srand(time(NULL));
	Board.init();
	Game::nextPiece = Game::generatePiece();
	Game::addPieceToBoard(Board, pieces);
	Game::minutesElapsed = 0;
	Game::secondsElapsed = 0;
	Game::score = 0;
	Game::delay = 0.7;
	Game::powerUpActiveTime = 10;
	Game::powerUpTimer = 0;
	Game::collectedPU = 0;
	Game::generatePowerUp(Board);
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
				if (Board.board[i][j].value == 4 || Board.board[i][j].value == 5 || Board.board[i][j].value == 6) {
					Game::collectedPU = Board.board[i][j].value;
					Game::powerUpTimer = 0;
					std::cout << "am schimbat collectedPU cu " << Game::collectedPU << std::endl;
				}
				if (i == 0 || Board.board[i - 1][j].value == 3 || Board.board[i - 1][j].value == 4 || Board.board[i - 1][j].value == 5 || Board.board[i - 1][j].value == 6) {

					Board.board[i][j].value = 0;
					Board.board[i][j].color = 0;
				}
				else Board.board[i][j] = Board.board[i - 1][j];
			}
}
void Game::moveRight(Board &Board) {
	for (int i = 0; i <ROWS; i++)
		for (int j = COLS - 1; j >= 0; j--)
			if (Board.board[i][j - 1].value == 1 || Board.board[i][j - 1].value == 2 || Board.board[i][j].value == 1 || Board.board[i][j].value == 2)
			{
				if (Board.board[i][j].value == 4 || Board.board[i][j].value == 5 || Board.board[i][j].value == 6) {
					Game::collectedPU = Board.board[i][j].value;
					Game::powerUpTimer = 0;
					std::cout << "am schimbat collectedPU cu " << Game::collectedPU << std::endl;
				}
				if (j == 0 || Board.board[i][j - 1].value == 3 || Board.board[i][j - 1].value == 4 || Board.board[i][j - 1].value == 5 || Board.board[i][j - 1].value == 6) {

					Board.board[i][j].value = 0;
					Board.board[i][j].color = 0;
				}
				else Board.board[i][j] = Board.board[i][j - 1];
			}
}
void Game::moveLeft(Board &Board) {
	for (int i = 0; i <ROWS; i++)
		for (int j = 0; j < COLS; j++)
			if (Board.board[i][j + 1].value == 1 || Board.board[i][j + 1].value == 2 || Board.board[i][j].value == 1 || Board.board[i][j].value == 2)
			{
				if (Board.board[i][j].value == 4 || Board.board[i][j].value == 5 || Board.board[i][j].value == 6) {
					Game::collectedPU = Board.board[i][j].value;
					Game::powerUpTimer = 0;
					std::cout << "am schimbat collectedPU cu " << Game::collectedPU << std::endl;
				}
				if (j == COLS - 1 || Board.board[i][j + 1].value == 3 || Board.board[i][j + 1].value == 4 || Board.board[i][j + 1].value == 5 || Board.board[i][j + 1].value == 6) {
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
			if (Board.board[i][j].value != 3 && Board.board[i][j].value != 4 && Board.board[i][j].value != 5) {
				Board.board[i][j].value = pieces[piece.x][piece.y][iP][jP];
				Board.board[i][j].color = piece.z;
			}
			else if ((Board.board[i][j].value == 4 || Board.board[i][j].value == 5) && (pieces[piece.x][piece.y][iP][jP] == 1 || pieces[piece.x][piece.y][iP][jP] == 2)) {
				Game::collectedPU = Board.board[i][j].value;
				std::cout << (int)Game::collectedPU << std::endl;
				Game::powerUpTimer = 0;
				std::cout << "am schimbat powerUp" << std::endl;
				Board.board[i][j].value = pieces[piece.x][piece.y][iP][jP];
				Board.board[i][j].color = piece.z;
			}
		}
}
void Game::drawInfo(sf::RenderWindow &window, HighScores HighScores) {
	Game::speed = 100 - Game::delay * 100;
	sf::Font gameInfoFont;
	gameInfoFont.loadFromFile("fonts/Roboto-Medium.ttf");
	sf::Text gameInfo[5];
	std::string gameScore = std::to_string(Game::score);
	std::string minutes = std::to_string((int)Game::minutesElapsed);
	if (Game::minutesElapsed <= 9)
		minutes = "0" + minutes;
	std::string seconds = std::to_string((int)Game::secondsElapsed);
	if (Game::secondsElapsed <= 9)
		seconds = "0" + seconds;
	std::string gameTime = minutes + ':' + seconds;
	std::string speed = std::to_string(Game::speed);

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

	gameInfo[2].setString(speed);
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

	if (scoreToBeat.score == Game::score) {
		scoreToBeat.minutesElapsed = Game::minutesElapsed;
		scoreToBeat.secondsElapsed = Game::secondsElapsed;
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

	if (Game::collectedPU == 4) {
		sf::Texture activePowerUpTexture;
		activePowerUpTexture.loadFromFile("images/tiles.png");
		sf::Sprite activePowerUp(activePowerUpTexture);
		activePowerUp.setTextureRect(sf::IntRect(Game::collectedPU % 4 * 18, 0, 18, 18));
		activePowerUp.setPosition(sf::Vector2f((OFFSET_X + COLS + 1) * 18 - 1, (OFFSET_Y + ROWS - 1) * 18));
		window.draw(activePowerUp);
	}
}
void Game::drawGameOver(sf::RenderWindow &window) {
	sf::Texture goBackground;
	goBackground.loadFromFile("images/gameOver.png");
	sf::Sprite goSprite(goBackground);
	goSprite.setTextureRect(sf::IntRect(0, 0, 720, 480));
	window.draw(goSprite);
}
void Game::generatePowerUp(Board &Board) {

	int powerUps[3] = { 4 , 5, 6 };
	int puX = rand() % COLS, puY;
	std::cout << "firstRow: " << Board.firstRow << std::endl;
	if (Board.firstRow >= 4)
		puY = rand() % Board.firstRow >= 4 ? rand() % Board.firstRow : 4;
	else
		puY = rand() % Board.firstRow;


	int puIndex = rand() % 2;

	for (int i = 0; i < 3; i++)
		std::cout << (int)powerUps[i] << " ";
	std::cout << std::endl;

	Board.board[puY][puX].value = powerUps[puIndex];
	std::cout << "am pus " << (int)powerUps[puIndex] << " pe " << puY << " / " << puX << std::endl;
}