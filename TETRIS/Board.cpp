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
void Board::clearLine(){
	bool clear = true;
	for(int i=0;i<COLS;i++)
		if (board[ROWS - 1][i] == 0) {
			clear = false;
			break;
		}
	if (clear) {
		for (int i = ROWS - 1; i > 0; i--) {
			for (int j = 0; i < COLS; j++)
				board[i][j] = board[i - 1][j];
		}
	}
}
void Board::Draw(sf::RenderWindow &window){
	sf::Texture blockBackground, gameBackground;
	blockBackground.loadFromFile("images/tiles.png");
	gameBackground.loadFromFile("images/gameBg.jpg");

	sf::Sprite block(blockBackground), background(gameBackground);
	block.setTextureRect(sf::IntRect(18, 0, 18, 18));
	background.setTextureRect(sf::IntRect(0, 0, window.getSize().x, window.getSize().y));
	window.draw(background);

	int levelOfDanger, topBlock=ROWS;
	for (int i = 0; i < ROWS; i++) {

		for (int j = 0; j < COLS; j++) {
			if (board[i][j] == 3) {
				topBlock = i;
				break;
			}
		}
		if (topBlock != -1) break;
	}
	if (topBlock > 16) levelOfDanger = 0;
	else if (topBlock > 13) levelOfDanger = 1;
	else if (topBlock > 9) levelOfDanger = 2;
	else if (topBlock > 5) levelOfDanger = 3;
	else if (topBlock > 3) levelOfDanger = 4;
	else levelOfDanger = 5;
	for (int i = 0; i < ROWS; i++)
		for (int j = 0; j < COLS; j++) {
			if (board[i][j] !=0) {
				block.setTextureRect(sf::IntRect(levelOfDanger*18, 0, 18, 18));
				block.setPosition(sf::Vector2f(j * 18+OFFSET_X*18, i * 18+OFFSET_Y*18));
				window.draw(block);
			}
		}
}