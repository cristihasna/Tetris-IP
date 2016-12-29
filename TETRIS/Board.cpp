#include "Board.h"
#include <time.h>
#include <stdlib.h>

Board::Board(sf::RenderWindow &window)
{

}


Board::~Board()
{
}

void Board::init() {
	for (int i = 0; i < ROWS; i++)
		for (int j = 0; j < COLS; j++)
			board[i][j] = 0;
}

bool Board::gameOver() {
	for (int i = 0; i < COLS; i++)
		if (board[0][i] == 3) return true;
	return false;
}

void Board::merge() {
	for (int i = 0; i < ROWS; i++)
		for (int j = 0; j < COLS; j++)
			if (board[i][j] == 1 || board[i][j] == 2)
				board[i][j] = 3;
}

void Board::clearLine(int &score){
	score += 10;
	int cleared = 0;
	int coeficient = 100;
	for (int i = ROWS - 1; i > 1; i--) {
		bool clear = true;
		while (clear) {
			for (int j = 0; j<COLS; j++)
				if (board[i][j] == 0) {
					clear = false;
					break;
				}
			if (clear) {
				for (int p = i; p > 1; p--)
					for (int j = 0; j < COLS; j++)
						board[p][j] = board[p - 1][j];
				cleared++;
			}
		}
	}
	score += cleared * cleared * coeficient;
}

void Board::Draw(sf::RenderWindow &window, sf::Vector2i nextPiece){
	sf::Texture blockBackground, gameBackground;
	blockBackground.loadFromFile("images/tiles.png");
	gameBackground.loadFromFile("images/gameBg.jpg");

	sf::Sprite block(blockBackground), movingBlock(blockBackground), background(gameBackground);
	block.setTextureRect(sf::IntRect(18, 0, 18, 18));
	movingBlock.setTextureRect(sf::IntRect(0, 0, 18, 18));
	background.setTextureRect(sf::IntRect(0, 0, window.getSize().x, window.getSize().y));
	window.draw(background);

	int levelOfDanger, topBlock=-1;
	for (int i = 0; i < ROWS; i++) {

		for (int j = 0; j < COLS; j++) {
			if (board[i][j] == 3) {
				topBlock = i;
				break;
			}
		}
		if (topBlock != -1) break;
	}
	if (topBlock > 16) levelOfDanger = 1;
	else if (topBlock > 13) levelOfDanger = 2;
	else if (topBlock > 9) levelOfDanger = 3;
	else if (topBlock > 5) levelOfDanger = 4;
	else if (topBlock > 3) levelOfDanger = 5;
	else levelOfDanger = 6;
	block.setTextureRect(sf::IntRect(levelOfDanger*18, 0, 18, 18));
	for (int i = 0; i < ROWS; i++)
		for (int j = 0; j < COLS; j++) {
			if (board[i][j] == 3) {
				block.setPosition(sf::Vector2f(j * 18+OFFSET_X*18, i * 18+OFFSET_Y*18));
				window.draw(block);
			}
			else if (board[i][j] == 1 || board[i][j]==2)
			{
				movingBlock.setPosition(sf::Vector2f(j * 18 + OFFSET_X * 18, i * 18 + OFFSET_Y * 18));
				window.draw(movingBlock);
			}
		}
	Pieces pieces(window);
	for(int i=0;i<5;i++)
		for (int j = 0; j < 5; j++) {
			if (pieces.pieces[nextPiece.x][nextPiece.y][i][j] != 0) {
				movingBlock.setPosition(sf::Vector2f((j + OFFSET_X + COLS + 1) * 18, (i + OFFSET_Y) * 18));
				window.draw(movingBlock);
			}
		}
}