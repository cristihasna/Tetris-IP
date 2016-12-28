#include "Game.h"
#include "Board.h"
#include "Pieces.h"
#include <iostream>


Game::Game(sf::RenderWindow &window)
{
}


Game::~Game()
{
}
void Game::init(Board &Board, Pieces Pieces) {
	Board.init();
	sf::Vector2i initialPiece = Game::generatePiece(Pieces);
	std::cout << "am gasit piesa " << initialPiece.x << " cu rotatia " << initialPiece.y << std::endl;
	sf::Vector2i initialPosition = Pieces.getInitialPosition(initialPiece.x, initialPiece.y);
	std::cout << "are pozitia initiala " << initialPosition.x << " / " << initialPosition.y << std::endl;

	for (int i = 0; i < 5; i++) {
		for (int j = 0, j2 = (COLS - 5) / 2; j < 5; j++, j2++) {
			if (Pieces.pieces[initialPiece.x][initialPiece.y][i][j] == 1 && Board.board[i][j2] == 0)
				Board.board[i][j2] = Pieces.pieces[initialPiece.x][initialPiece.y][i][j];
		}
	}
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++)
			std::cout << Board.board[i][j] << " ";
		std::cout << std::endl;
	}
}
sf::Vector2i Game::generatePiece(Pieces Pieces) {
	srand(time(NULL));
	int piece = rand() % 7;
	int rotation = rand() % 4;
	sf::Vector2i result;
	result.x = piece;
	result.y = rotation;
	return result;
}
