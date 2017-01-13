#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
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

	Music intro, inGameIntro, move, clearLine, clearPieces, holdPiece;
	intro.openFromFile("sounds/intro.ogg");
	inGameIntro.openFromFile("sounds/intro_game.ogg");
	move.openFromFile("sounds/move.ogg");
	clearLine.openFromFile("sounds/clear_line.ogg");
	clearPieces.openFromFile("sounds/clear_line_2.ogg");
	holdPiece.openFromFile("sounds/hold.ogg");
	Menu menu(window.getSize().x, window.getSize().y);
	HighScores highScores(window);
	Board board(window);
	Game game(window);

	intro.play();

	Clock clock;

	float timer = 0, generatePowerUpTimer=0;
	bool isMenuActive = true,
		isGameActive = false,
		isHighScoresActive = false,
		isGamePaused = false,
		scoreProcessed = false;

	int scoreChanged = 0;

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
							inGameIntro.play();
							scoreChanged = 0;
							scoreProcessed = false;
							highScores.readHighScores(highScores.highScores);
							clock.restart();
						}

						else if (menu.getSelectedMenuItem() == 1) {
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
							isHighScoresActive = false;
							isMenuActive = true;
						}
						if (highScores.getSelectedMenuItem() == 1) {
							highScores.MoveUp();
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
							isGamePaused = false;
						}
						if (e.key.code == Keyboard::Return) {
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
										if (game.collectedPU != 6) timer = 0;
										move.play();
									}
								}
								else {
									if (game.checkRight(board)) {
										game.moveRight(board);
										if(game.collectedPU!=6) timer = 0;
										move.play();
									}
								}
							}
							if (e.key.code == Keyboard::Right) {
								if (game.collectedPU != 4) {
									if (game.checkRight(board)) {
										game.moveRight(board);
										if(game.collectedPU!=6) timer = 0;
										move.play();
									}
								}
								else {
									if (game.checkLeft(board)) {
										game.moveLeft(board);
										if (game.collectedPU != 6) timer = 0;
										move.play();
									}
								}
							}

							if (e.key.code == Keyboard::Down && game.collectedPU!=6) {
								if (game.checkDown(board)) {
									game.moveDown(board);
									timer = 0;
									move.play();
								}
							}
							if (e.key.code == Keyboard::Up && game.collectedPU!=6) {
								if (game.checkRotate(board, game.currentPiece, pieces)) {
									game.Rotate(board, game.currentPiece, pieces);
									timer = 0;
									move.play();
								}
							}
							if (e.key.code == Keyboard::Space && game.collectedPU!=6) {
								timer = 0;
								while (game.checkDown(board)) {
									game.moveDown(board);
								}
								move.play();
							}
							if ((e.key.code == Keyboard::LControl || e.key.code == Keyboard::RControl)  && !game.isPieceHold && game.collectedPU!=6) {
								game.holdPiece(board, pieces);
								timer = 0;
								holdPiece.play();
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

				for (int i = 0; i<ROWS; i++)
					for (int j = 0; j < COLS; j++) {
						if (board.board[i][j].value == 4 || board.board[i][j].value == 5 || board.board[i][j].value == 6) {
							board.board[i][j].timer += time;
							if (board.board[i][j].timer >= game.powerUpVisibleTime) {
								board.board[i][j].timer = 0;
								board.board[i][j].value = 0;
							}
						}
					}

				generatePowerUpTimer += time;
				if (generatePowerUpTimer >= game.generatePowerUpDelay) {
					game.generatePowerUp(board, game.generatePowerUpDelay);
					generatePowerUpTimer = 0;
				}

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
						move.play();
						if (game.collectedPU == 6) {
							if (game.checkRotate(board, game.currentPiece, pieces))
							game.Rotate(board, game.currentPiece, pieces);
						}
					}
				}
				else {
					if (timer > game.delay) {
						game.isPieceHold = false;
						board.merge();
						int scoreBefore = game.score;
						board.clearLine(game.score);
						if (scoreBefore != game.score+10)
							clearLine.play();

						if (game.collectedPU == 6) {
							game.collectedPU = 0;
							game.powerUpTimer = 0;
						}
							
						if (!board.gameOver()) {
							game.addPieceToBoard(board, pieces);
						}
						if (game.score >= 500 && scoreChanged == 0) {
							scoreChanged = 1;
							game.delay /= 1.6;
							game.generatePowerUp(board, game.generatePowerUpDelay);
						}
						if (game.score >= 1000 && scoreChanged == 1) {
							scoreChanged = 2;
							game.delay /= 1.3;

							game.generatePowerUp(board, game.generatePowerUpDelay);

						}
						if (game.score >= 1500 && scoreChanged == 2) {
							scoreChanged = 3;
							game.delay /= 1.3;

							game.generatePowerUp(board, game.generatePowerUpDelay);

						}
						if (game.score >= 2000 && scoreChanged == 3) {
							scoreChanged = 4;
							game.delay /= 1.3;

							game.generatePowerUp(board, game.generatePowerUpDelay);

						}
						if (game.score >= 2500 && scoreChanged == 4) {
							scoreChanged = 5;
							game.delay /= 1.3;

							game.generatePowerUp(board, game.generatePowerUpDelay);

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

			board.Draw(window, game.nextPiece, game.heldPiece, pieces);
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