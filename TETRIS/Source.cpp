#include <SFML\Graphics.hpp>
#include <ctime>
#include <iostream>
#include "Menu.h"
#include "HighScores.h"
#include "Board.h"
#include "Pieces.h"
#include "Game.h"


using namespace std;
using namespace sf;



int main() {

	float x;
	x = 0;
	cout << (int)x << endl;
	RenderWindow window(VideoMode(720, 480), "Tetris", Style::Close);
	//window.setMouseCursorVisible(false);
	window.setFramerateLimit(15);
	
	Menu menu(window.getSize().x, window.getSize().y);
	HighScores highScores(window);
	Board board(window);
	Pieces pieces(window);
	Game game(window);

	Clock clock;

	float timer = 0;
	bool isMenuActive = true,
		isGameActive = false,
		isHighScoresActive = false,
		isGamePaused = false,
		scoreProcessed = false;

	int scoreChanged = 0;

	while (window.isOpen()) {
		
		Event e;
		while (window.pollEvent(e)) {

			if (e.type == Event::MouseButtonPressed) {
				if (e.key.code == sf::Mouse::Button::Left) {
					sf::Vector2i mousePos = sf::Mouse::getPosition();
					cout << "clicked pe pozitia " << mousePos.x << " / " << mousePos.y << endl;

				}
			}


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
							scoreChanged = 0;
							scoreProcessed = false;
							clock.restart();

						}

						else if (menu.getSelectedMenuItem() == 1) {
							cout << "High Scores" << endl;
							isMenuActive = false;
							isHighScoresActive = true;
							highScores.readHighScores(highScores.highScores);
						}

						else if (menu.getSelectedMenuItem() == 2) {
							window.close();
						}
							
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
							highScores.resetScores(highScores.highScores);
							cout << "am sters scorurile" << endl;
							isHighScoresActive = false;
							isMenuActive = true;
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
					if (isGamePaused) {
						if (e.key.code == Keyboard::Escape) {
							cout << "am apasat esc" << endl;
							isGamePaused = false;
						}
						if (e.key.code == Keyboard::Return) {
							cout << "am apasat ender" << endl;
							isGamePaused = false;
							isGameActive = false;
							isMenuActive = true;

							if (!scoreProcessed) {
								highScores.processScore(game.score, game.minutesElapsed, game.secondsElapsed, highScores.highScores);
								scoreProcessed = true;
							}
						}
					}
					else {
						if (e.key.code == Keyboard::Escape) {
							cout << "am apasat esc" << endl;
							isGamePaused = true;
						}
						else if (!board.gameOver() && !isGamePaused) {

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
					
				}

			}
				
			if (e.type == Event::Closed)
				window.close();
		}

		if (isGameActive == true) {
			float time = clock.getElapsedTime().asSeconds();
			clock.restart();
			timer += time;
			if (!board.gameOver() && !isGamePaused) {
				game.secondsElapsed += time;
				if (game.secondsElapsed >= 60) {
					game.secondsElapsed = 0;
					game.minutesElapsed++;
				}
			}
			
			if(!board.gameOver() && !isGamePaused) {
				if(game.checkDown(board)) {
					if (timer > game.delay) {
						timer = 0;
						game.moveDown(board);
					}
				}
				else {
					if (timer > game.delay) {
						board.merge();
						board.clearLine(game.score);
						if(!board.gameOver()) game.addPieceToBoard(board, pieces);
						if (game.score >= 500 && scoreChanged==0 ) {scoreChanged = 1;
							game.delay /= 1.6;
							game.generatePowerUp(board, pieces);
							std::cout << "Delay : " << game.delay << " ScoreChanged: " << scoreChanged << std::endl;
							
						}
						if (game.score >= 1000 && scoreChanged == 1) {
							scoreChanged = 2;
							game.delay/=1.3;

							game.generatePowerUp(board, pieces);
							std::cout << "Delay : " << game.delay << " ScoreChanged: " << scoreChanged << std::endl;
							
						}
						if(game.score >= 1500 && scoreChanged == 2) {
							scoreChanged = 3;
							game.delay /= 1.3;

							game.generatePowerUp(board, pieces);
							std::cout << "Delay : " << game.delay << " ScoreChanged: " << scoreChanged << std::endl;
							
						}
						if (game.score >= 2000 && scoreChanged == 3) {
							scoreChanged = 4;
							game.delay /= 1.3;

							game.generatePowerUp(board, pieces);
							std::cout << "Delay : " << game.delay << " ScoreChanged: " << scoreChanged << std::endl;

						}
						if (game.score >= 2500 && scoreChanged == 4) {
							scoreChanged = 5;
							game.delay /= 1.3;

							game.generatePowerUp(board, pieces);
							std::cout << "Delay : " << game.delay << " ScoreChanged: " << scoreChanged << std::endl;

						}
						timer = 0;
					}
				}
			}

		}

		window.clear(Color::White);

		if (isMenuActive) 
			menu.Draw(window);

		else if (isGameActive) {

			board.Draw(window, game.nextPiece);
			game.drawInfo(window);

			if (isGamePaused) {
				sf::Texture pausedGameBg;
				pausedGameBg.loadFromFile("images/gamePaused.png");
				sf::Sprite pausedGame(pausedGameBg);
				pausedGame.setTextureRect(sf::IntRect(0, 0, window.getSize().x, window.getSize().y));
				window.draw(pausedGame);
			}

			if (board.gameOver()) {
				if (!scoreProcessed) {
					cout << "intra aici " << endl;
					scoreProcessed = true;
					char timestamp[100] = "__";
					highScores.processScore(game.score, game.minutesElapsed, game.secondsElapsed, highScores.highScores);
				}
				game.drawGameOver(window);
			}

		}
		else if (isHighScoresActive) {
			highScores.Draw(window);
		}
				
		window.display();
		sf::Time sleepTime;
	}

	return 0;
}