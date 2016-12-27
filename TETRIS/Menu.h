#pragma once
#include <SFML\Graphics.hpp>

#define MAX_NUM_ITEMS 3
class Menu
{
public:
	Menu(float width, float height);
	~Menu();

	void Draw(sf::RenderWindow &window);
	void MoveUp();
	void MoveDown();

	int getSelectedMenuItem() {
		return selectedItemIndex;
	}

private:

	int selectedItemIndex;
	sf::Font font1, font2;
	sf::Text menu[MAX_NUM_ITEMS];

};

