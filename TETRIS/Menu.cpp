#include "Menu.h"
#include <iostream>

#define customBlue sf::Color (134, 181, 225)
#define customBlack sf::Color (55, 65, 64)

Menu::Menu(float width, float height)
{
	if (!font1.loadFromFile("fonts/Roboto-Medium.ttf") || !font2.loadFromFile("fonts/BAUHS93.ttf")) {
		std::cout << "Nu s-a putut incarca Roboto-Medium.ttf" << std::endl;

	}
	
	menu[0].setFont(font2);
	menu[0].setFillColor(customBlue);
	menu[0].setString("Play");
	menu[0].setCharacterSize(72);
	menu[0].setPosition(sf::Vector2f((width-140) / 2, height / (MAX_NUM_ITEMS + 1) * 1 +60));

	menu[1].setFont(font2);
	menu[1].setFillColor(customBlack);
	menu[1].setString("High Scores");
	menu[1].setCharacterSize(28);
	menu[1].setPosition(sf::Vector2f((width - 140) / 2, height / (MAX_NUM_ITEMS + 1) * 1.7 +60));

	menu[2].setFont(font2);
	menu[2].setFillColor(customBlack);
	menu[2].setString("     Quit");
	menu[2].setPosition(sf::Vector2f((width - 140) / 2, height / (MAX_NUM_ITEMS + 1) * 2 +60));

	selectedItemIndex = 0;
	
}


Menu::~Menu()
{
}


void Menu::Draw(sf::RenderWindow &window) {
	sf::Texture menuBackground;
	menuBackground.loadFromFile("images/menuBg.jpg");
	sf::Sprite background(menuBackground);

	background.setTextureRect(sf::IntRect(0, 0, window.getSize().x, window.getSize().y));
	window.draw(background);
	for (int i = 0; i < MAX_NUM_ITEMS; i++)
		window.draw(menu[i]);
}
void Menu::MoveUp() {
	if (selectedItemIndex - 1 >= 0) {
		menu[selectedItemIndex].setFillColor(customBlack);
		selectedItemIndex--;
		menu[selectedItemIndex].setFillColor(customBlue);
	}
	else {
		menu[selectedItemIndex].setFillColor(customBlack);
		selectedItemIndex = MAX_NUM_ITEMS - 1;

		menu[selectedItemIndex].setFillColor(customBlue);
	}
}
void Menu::MoveDown() {
	if (selectedItemIndex + 1 < MAX_NUM_ITEMS) {
		menu[selectedItemIndex].setFillColor(customBlack);
		selectedItemIndex++;
		menu[selectedItemIndex].setFillColor(customBlue);
	}
	else {
		menu[selectedItemIndex].setFillColor(customBlack);
		selectedItemIndex = 0;

		menu[selectedItemIndex].setFillColor(customBlue);
	}
}