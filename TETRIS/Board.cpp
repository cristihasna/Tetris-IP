#include "Board.h"
#include <time.h>
#include <iostream>
#include <stdlib.h>

Board::Board(sf::RenderWindow &window)
{

}


Board::~Board()
{
}

void Board::init() {
	Board::firstRow = ROWS-1;
	for (int i = 0; i < ROWS; i++)
		for (int j = 0; j < COLS; j++) {
			board[i][j].value = 0;
			board[i][j].color = 0;
		}
			
}

bool Board::gameOver() {
	for (int i = 0; i < COLS; i++)
		if (board[0][i].value == 3) return true;
	return false;
}

void Board::merge() {
	for (int i = 0; i < ROWS; i++)
		for (int j = 0; j < COLS; j++)
			if (board[i][j].value == 1 || board[i][j].value == 2)
				board[i][j].value = 3;
}

void Board::clearLine(int &score){
	std::cout << "scor inainte: " << score;
	score += 10;
	int cleared = 0;
	int coeficient = 100;
	for (int i = ROWS - 1; i > 1; i--) {
		bool clear = true;
		while (clear) {
			for (int j = 0; j<COLS; j++)
				if (board[i][j].value != 3) {
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
	std::cout << "scor dupa: " << score << std::endl;
}

void Board::Draw(sf::RenderWindow &window, sf::Vector3i nextPiece, char pieces[7][4][5][5]){
	sf::Texture blockBackground, gameBackground;
	blockBackground.loadFromFile("images/tiles.png");
	gameBackground.loadFromFile("images/gameBg.jpg");

	sf::Sprite block(blockBackground), background(gameBackground);

	sf::Sprite powerUpBlock(blockBackground);

	background.setTextureRect(sf::IntRect(0, 0, window.getSize().x, window.getSize().y));
	window.draw(background);

	int levelOfDanger, topBlock=-1;
	for (int i = 0; i < ROWS; i++) {

		for (int j = 0; j < COLS; j++) {
			if (board[i][j].value == 3) {
				topBlock = i;
				break;
			}
		}
		if (topBlock != -1) break;
	}
	if (topBlock == -1) topBlock = ROWS - 1;
	if(topBlock != -1) firstRow = topBlock;
	if (topBlock > 15) levelOfDanger = 0;
	else if (topBlock > 10) levelOfDanger = 1;
	else if (topBlock > 5) levelOfDanger = 2;
	else levelOfDanger = 3;
	for (int i = 0; i < ROWS; i++)
		for (int j = 0; j < COLS; j++) {
			if (board[i][j].value == 3 || board[i][j].value == 1 || board[i][j].value == 2) {
				block.setTextureRect(sf::IntRect(levelOfDanger * 18, (board[i][j].color+1)*18, 18, 18));
				block.setPosition(sf::Vector2f(j * 18+OFFSET_X*18, i * 18+OFFSET_Y*18));
				window.draw(block);
			}
			else if (board[i][j].value == 4 || board[i][j].value == 5 || board[i][j].value == 6) {
				powerUpBlock.setTextureRect(sf::IntRect(board[i][j].value%4*18, 0, 18, 18));
				powerUpBlock.setPosition(sf::Vector2f(j * 18 + OFFSET_X * 18, i * 18 + OFFSET_Y * 18));
				window.draw(powerUpBlock);
			}
		}
	for(int i=0;i<5;i++)
		for (int j = 0; j < 5; j++) {
			if (pieces[nextPiece.x][nextPiece.y][i][j] != 0) {
				block.setTextureRect(sf::IntRect(levelOfDanger * 18, (nextPiece.z + 1) * 18, 18, 18));
				block.setPosition(sf::Vector2f((j + OFFSET_X + COLS + 1) * 18, (i + OFFSET_Y) * 18));
				window.draw(block);
			}
		}
}