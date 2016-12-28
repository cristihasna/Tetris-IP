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
void Game::addPieceToBoard(Board &Board, Pieces Pieces) {
		actualPiece = nextPiece;
		nextPiece = Game::generatePiece(Pieces);
	sf::Vector2i initialPos = Pieces.getInitialPosition(actualPiece.x, actualPiece.y);
	for (int i = 0; i < 5-initialPos.x; i++) {
		for (int j = 0, j2 = (COLS - 5) / 2+ initialPos.y; j < 5-initialPos.y; j++, j2++) {
			if (Pieces.pieces[actualPiece.x][actualPiece.y][i+initialPos.x][j+initialPos.y] == 1 || Pieces.pieces[actualPiece.x][actualPiece.y][i + initialPos.x][j + initialPos.y] == 2 && Board.board[i][j2] == 0)
				Board.board[i][j2] = Pieces.pieces[actualPiece.x][actualPiece.y][i+initialPos.x][j+initialPos.y];
		}
	}
	nextPiece = Game::generatePiece(Pieces);
}
void Game::init(Board &Board, Pieces Pieces) {
	Board.init();
	sf::Vector2i initialPosition = Pieces.getInitialPosition(actualPiece.x, actualPiece.y);
	Game::addPieceToBoard(Board, Pieces);
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

bool Game::checkDown(Board &Board) {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			if (Board.board[i][j] == 1 || Board.board[i][j] == 2) {
				if (i == ROWS - 1) {
					return false;
				}
				else if (Board.board[i+1][j]==3) {
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
			if (Board.board[i][j] == 1 || Board.board[i][j] == 2) {
				if (j == 0) {
					return false;
				}
				else if (Board.board[i][j-1] == 3) {
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
			if (Board.board[i][j] == 1 || Board.board[i][j] == 2) {
				if (j == COLS - 1) {
					return false;
				}
				else if (Board.board[i][j+1] == 3) {
					return false;
				}
			}
		}
	}
	return true;
}
bool Game::checkRotate(Board &Board, Pieces Pieces, sf::Vector2i piece) {
	int pivotX=0, pivotY=0;
	for (int i = 0; i < ROWS && (pivotX==0 && pivotY == 0); i++) {
		for(int j=0;j<COLS && (pivotX == 0 && pivotY == 0); j++)
			if (Board.board[i][j] == 2) {
				pivotX = j;
				pivotY = i;
			}
	}
	int nextRotation = (piece.y + 1)%4;
	for (int i = pivotY - 2, iP = 0; i <= pivotY + 2; i++, iP++)
		for (int j = pivotX - 2, jP = 0; j <= pivotX + 2; j++, jP++)
			if ((i>=ROWS || j>=COLS || j <0 || i<0 || Board.board[i][j]== 3) && (Pieces.pieces[piece.x][nextRotation][iP][jP] == 1 || Pieces.pieces[piece.x][nextRotation][iP][jP] == 2))
				return false;
	return true;
}

void Game::moveDown(Board &Board) {
	for (int i = ROWS - 1; i >= 0; i--)
		for (int j = 0; j < COLS; j++)
			if (Board.board[i - 1][j] == 1 || Board.board[i - 1][j] == 2 || Board.board[i][j] == 1 || Board.board[i][j] == 2)
			{
				if (i == 0 || Board.board[i-1][j]==3) Board.board[i][j] = 0;
				else Board.board[i][j] = Board.board[i - 1][j];
			}
}
void Game::moveRight(Board &Board) {
	for (int i = 0; i <ROWS; i++)
		for (int j = COLS - 1; j >= 0; j--)
			if (Board.board[i][j-1] == 1 || Board.board[i][j-1] == 2 || Board.board[i][j] == 1 || Board.board[i][j] == 2)
			{
				if (j == 0 || Board.board[i][j - 1] == 3) Board.board[i][j] = 0;
				else Board.board[i][j] = Board.board[i][j-1];
			}
}
void Game::moveLeft(Board &Board) {
	for (int i = 0; i <ROWS; i++)
		for (int j = 0; j < COLS; j++)
			if (Board.board[i][j + 1] == 1 || Board.board[i][j + 1] == 2 || Board.board[i][j] == 1 || Board.board[i][j] == 2)
			{
				if (j == COLS-1 || Board.board[i][j + 1]==3) Board.board[i][j] = 0;
				else Board.board[i][j] = Board.board[i][j + 1];
			}
}
void Game::Rotate(Board &Board, Pieces Pieces, sf::Vector2i &piece) {
	int pivotX = 0, pivotY = 0;
	for (int i = 0; i < ROWS && (pivotX == 0 && pivotY == 0); i++) {
		for (int j = 0; j<COLS && (pivotX == 0 && pivotY == 0); j++)
			if (Board.board[i][j] == 2) {
				pivotX = j;
				pivotY = i;
			}
	}
	piece.y = (piece.y + 1) % 4;
	for (int i = pivotY - 2, iP = 0; i <= pivotY + 2; i++, iP++)
		for (int j = pivotX - 2, jP = 0; j <= pivotX + 2; j++, jP++)
		{
			if (Board.board[i][j] != 3)
				Board.board[i][j] = Pieces.pieces[piece.x][piece.y][iP][jP];
		}
}