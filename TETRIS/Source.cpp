#include <SFML\Graphics.hpp>
#include <time.h>
#include <iostream>
#include "Menu.h"

using namespace std;
using namespace sf;

#define moveSpeed 18
#define delay 0.5
#define M 20
#define N 26

void setPositionDown(Vector2f &vector) {
	vector.y += moveSpeed;
}
bool checkIfBottom(int piece, Sprite &sprite, Vector2f positionDown, Vector2f windowSize) {

	return true;
}
void moveDown(int piece[5][5], Sprite &sprite, Vector2f positionDown, RenderWindow &window) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (piece[i][j]) {
				int posY = i * 18;
				int posX = j * 18;
				sprite.setPosition(Vector2f(posX + positionDown.x, posY + positionDown.y));
				window.draw(sprite);
			}
		}
	}
}

int main() {
	RenderWindow window(VideoMode(480, 480), "Tetris", Style::Close);
	
	Menu menu(window.getSize().x, window.getSize().y);
	
	int piece[5][5] = {
		0, 0, 0, 0, 0,
		0, 0, 1, 0, 0,
		0, 1, 1, 1, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0
	};
	int board[M][N] = { 0 };

	Vector2f positionDown;
	positionDown.x = 0;
	positionDown.y = 0;
	Texture t;
	t.loadFromFile("images/tiles.png");

	Sprite sprite(t);
	sprite.setTextureRect(IntRect(0, 0, 18, 18));
	sprite.setOrigin(Vector2f(9, 9));
	sprite.setPosition(Vector2f(9, 9));
	Clock clock;
	float timer = 0;
	bool left = 0, right = 0;
	bool isMenuActive = true,
		isGameActive = false;
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
							isMenuActive = false;
							isGameActive = true;
						}
						if (menu.getSelectedMenuItem() == 2)
							window.close();
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

			Vector2u windowSize = window.getSize();
			Vector2f spritePos;
			if (Keyboard::isKeyPressed(Keyboard::Key::Down)) {
				if (spritePos.y < windowSize.y - 9) {
					positionDown.y += 18;
					timer = 0;
				}
			}
			if (spritePos.y < windowSize.y - 9) {
				if (timer >= delay) {
					if (!left && !right)
						setPositionDown(positionDown);
					left = false;
					right = false;
					timer = 0;
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::Key::Left) && left == false) {
				positionDown.x -= 18;
				timer = 0;
				left = true;
			}
			if (Keyboard::isKeyPressed(Keyboard::Key::Right) && right == false) {
				positionDown.x += 18;
				timer = 0;
				right = true;
			}
		}

		window.clear(Color::White);
		if (isMenuActive) 
			menu.draw(window);
		else {
			if(isGameActive)
			moveDown(piece, sprite, positionDown, window);
		}
		window.display();
	}

	return 0;
}