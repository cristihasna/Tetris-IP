#include <SFML\Graphics.hpp>
#include <time.h>
using namespace sf;

int main() {
	RenderWindow window(VideoMode(320, 480), "Tetris", Style::Close);

	Texture t;
	t.loadFromFile("images/tiles.png");

	Sprite sprite(t);
	sprite.setTextureRect(IntRect(0, 0, 18, 18));

	while (window.isOpen()) {
		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed)
				window.close();
		}
		window.clear(Color::White);
		window.draw(sprite);
		window.display();
	}

	return 0;
}