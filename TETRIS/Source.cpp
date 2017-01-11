#include <SFML\Graphics.hpp>
#include <ctime>
#include <iostream>
#include "Menu.h"
#include "HighScores.h"
#include "Game.h"
#include "Pieces.h"


using namespace std;
using namespace sf;



int main() {


	RenderWindow window(VideoMode(720, 480), "Tetris", Style::Close);
	window.setMouseCursorVisible(false);

	Menu menu(window.getSize().x, window.getSize().y);
	HighScores highScores(window);
	Board board(window);
	Game game(window);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 5; j++) {
			for (int p = 0; p < 5; p++)
				cout << (int)pieces[6][i][j][p] << " ";
			cout << endl;
		}
		cout << "-------------" << endl;
	}

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
							highScores.readHighScores(highScores.highScores);
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
							if (board.gameOver()) {

								isGamePaused = false;
								isGameActive = false;
								isMenuActive = true;

								if (!scoreProcessed) {
									highScores.processScore(game.score, game.minutesElapsed, game.secondsElapsed, highScores.highScores);
									scoreProcessed = true;
								}
							}
							else isGamePaused = true;
						}
						else if (!board.gameOver() && !isGamePaused) {


							if (e.key.code == Keyboard::Left) {
								if (game.collectedPU != 4) {
									if (game.checkLeft(board)) {
										game.moveLeft(board);
										timer = 0;
									}
								}
								else {
									if (game.checkRight(board)) {
										game.moveRight(board);
										timer = 0;
									}
								}
							}
							if (e.key.code == Keyboard::Right) {
								if (game.collectedPU != 4) {
									if (game.checkRight(board)) {
										game.moveRight(board);
										timer = 0;
									}
								}
								else {
									if (game.checkLeft(board)) {
										game.moveLeft(board);
										timer = 0;
									}
								}
							}

							if (e.key.code == Keyboard::Down) {
								if (game.checkDown(board)) {
									game.moveDown(board);
									timer = 0;
								}
							}
							if (e.key.code == Keyboard::Up) {
								if (game.checkRotate(board, game.actualPiece, pieces)) {
									game.Rotate(board, game.actualPiece, pieces);
									timer = 0;
									cout << game.score << endl;
								}
							}
							if (e.key.code == Keyboard::Space) {
								timer = 0;
								while (game.checkDown(board)) {
									game.moveDown(board);
								}
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

				if (game.collectedPU != 0) {
					game.powerUpTimer += time;
					if (game.powerUpTimer >= game.powerUpActiveTime) {
						game.powerUpTimer = 0;
						game.collectedPU = 0;
					}
				}

				game.secondsElapsed += time;
				if (game.secondsElapsed >= 60) {
					game.secondsElapsed = 0;
					game.minutesElapsed++;
				}
			}

			if (!board.gameOver() && !isGamePaused) {
				if (game.checkDown(board)) {
					if (timer > game.delay) {
						timer = 0;
						game.moveDown(board);
					}
				}
				else {
					if (timer > game.delay) {
						board.merge();
						board.clearLine(game.score);
						if (!board.gameOver()) {
							game.addPieceToBoard(board, pieces);
						}
						if (game.score >= 500 && scoreChanged == 0) {
							scoreChanged = 1;
							game.delay /= 1.6;
							game.generatePowerUp(board);
							std::cout << "Delay : " << game.delay << " ScoreChanged: " << scoreChanged << std::endl;
						}
						if (game.score >= 1000 && scoreChanged == 1) {
							scoreChanged = 2;
							game.delay /= 1.3;

							game.generatePowerUp(board);
							std::cout << "Delay : " << game.delay << " ScoreChanged: " << scoreChanged << std::endl;

						}
						if (game.score >= 1500 && scoreChanged == 2) {
							scoreChanged = 3;
							game.delay /= 1.3;

							game.generatePowerUp(board);
							std::cout << "Delay : " << game.delay << " ScoreChanged: " << scoreChanged << std::endl;

						}
						if (game.score >= 2000 && scoreChanged == 3) {
							scoreChanged = 4;
							game.delay /= 1.3;

							game.generatePowerUp(board);
							std::cout << "Delay : " << game.delay << " ScoreChanged: " << scoreChanged << std::endl;

						}
						if (game.score >= 2500 && scoreChanged == 4) {
							scoreChanged = 5;
							game.delay /= 1.3;

							game.generatePowerUp(board);
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

			board.Draw(window, game.nextPiece, pieces);
			game.drawInfo(window, highScores);

			if (isGamePaused) {
				sf::Texture pausedGameBg;
				pausedGameBg.loadFromFile("images/gamePaused.png");
				sf::Sprite pausedGame(pausedGameBg);
				pausedGame.setTextureRect(sf::IntRect(0, 0, window.getSize().x, window.getSize().y));
				window.draw(pausedGame);
			}

			if (board.gameOver()) {
				if (!scoreProcessed) {
					scoreProcessed = true;
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