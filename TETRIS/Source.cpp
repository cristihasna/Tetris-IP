#include <SFML\Graphics.hpp>
#include <time.h>
#include <iostream>
#include "Menu.h"
#include "HighScores.h"
#include "Board.h"
#include "Pieces.h"
#include "Game.h"

#define delay 0.5

using namespace std;
using namespace sf;

int main() {


	RenderWindow window(VideoMode(720, 480), "Tetris", Style::Close);
	
	Menu menu(window.getSize().x, window.getSize().y);
	HighScores highScores(window);
	Board board(window);
	Pieces pieces(window);
	Game game(window);

	float score = 0;

	Clock clock;

	float timer = 0;
	bool left = 0, right = 0;
	bool isMenuActive = true,
		isGameActive = false,
		isHighScoresActive = false;
	while (window.isOpen()) {
		
		Event e;
		while (window.pollEvent(e)) {
			if (isMenuActive) {
				if (e.type == Event::KeyPressed) {

					if (e.key.code == Keyboard::Up)
						menu.MoveUp();

					if (e.key.code == Keyboard::Down)
						menu.MoveDown();

					if (e.key.code == Keyboard::Return) {

						if (menu.getSelectedMenuItem() == 0) {
							isGameActive = true;
							isMenuActive = false;
							game.init(board, pieces);

						}

						else if (menu.getSelectedMenuItem() == 1) {
							cout << "High Scores" << endl;
							isMenuActive = false;
							isHighScoresActive = true;
						}

						else if (menu.getSelectedMenuItem() == 2)
							window.close();
					}
				}

			}

			else if (isHighScoresActive) {
				if (e.type == Event::KeyPressed) {
					if (e.key.code == Keyboard::Up)
						highScores.MoveUp();
					if (e.key.code == Keyboard::Down)
						highScores.MoveDown();
					if (e.key.code == Keyboard::Return) {
						if (highScores.getSelectedMenuItem() == 0) {
							//highScores.resetScores();
							cout << "am sters scorurile" << endl;
						}
						if (highScores.getSelectedMenuItem() == 1) {
							highScores.MoveUp();
							cout << "Am trecut la meniu principal" << endl;
							isHighScoresActive = false;
							isMenuActive = true;
						}
					}
				}

			}
			else if (isGameActive) {
				if (e.type == Event::KeyPressed) {
					if (e.key.code == Keyboard::Escape) {
						cout << "am apasat esc" << endl;
						isGameActive = false;
						isMenuActive = true;
					}
					if (e.key.code == Keyboard::Left) {
						if (game.checkLeft(board)) {
							game.moveLeft(board);
							timer = 0;
						}
					}
					if (e.key.code == Keyboard::Right) {
						if (game.checkRight(board)) {
							game.moveRight(board);
							timer = 0;
						}
					}

					if (e.key.code == Keyboard::Down) {
						if (game.checkDown(board)) {
							game.moveDown(board);
							timer = 0;
						}
					}
					if (e.key.code == Keyboard::Up) {
						if (game.checkRotate(board, pieces, game.actualPiece)) {
							game.Rotate(board, pieces, game.actualPiece);
							timer = 0;
						}
					}
					if (e.key.code == Keyboard::Space) {
						while (game.checkDown(board)) {
							game.moveDown(board);
						}
						timer = 0;
					}
				}

			}
				
			if (e.type == Event::Closed)
				window.close();
		}

		if (isGameActive == true) {
			float time = clock.getElapsedTime().asSeconds();
			clock.restart();
			timer += time;
			if(!board.gameOver()) {
				if(game.checkDown(board)) {
					if (timer > delay) {
						timer = 0;
						game.moveDown(board);
					}
				}
				else {
					if (timer > delay) {
						board.merge();
						board.clearLine(score);
						std::cout << score << std::endl;
						if(!board.gameOver()) game.addPieceToBoard(board, pieces);
						timer = 0;
					}
				}
			}

		}

		window.clear(Color::Black);

		if (isMenuActive) 
			menu.Draw(window);

		else if (isGameActive) {
			board.Draw(window, game.nextPiece);
		}

		else if (isHighScoresActive) {
			highScores.Draw(window);
		}
				
		window.display();
	}

	return 0;
}