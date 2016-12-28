#include <SFML\Graphics.hpp>
#include <time.h>
#include <iostream>
#include "Menu.h"
#include "HighScores.h"
#include "Board.h"
#include "Pieces.h"
#include "Game.h"

using namespace std;
using namespace sf;

int main() {


	RenderWindow window(VideoMode(720, 480), "Tetris", Style::Close);
	
	Menu menu(window.getSize().x, window.getSize().y);
	HighScores highScores(window);
	Board board(window);
	Pieces pieces(window);
	Game game(window);


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
							cout << "am dat drumu la joc" << endl;
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
				}

			}
				
			if (e.type == Event::Closed)
				window.close();
		}

		if (isGameActive == true) {
			float time = clock.getElapsedTime().asSeconds();
			clock.restart();
			timer += time;

		}

		window.clear(Color::Black);

		if (isMenuActive) 
			menu.Draw(window);

		else if (isGameActive) {
			board.Draw(window);
		}

		else if (isHighScoresActive) {
			highScores.Draw(window);
		}
				
		window.display();
	}

	return 0;
}