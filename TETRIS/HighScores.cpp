#include "HighScores.h"
#include <fstream>

#define customBlue sf::Color (134, 181, 225)
#define customBlack sf::Color (55, 65, 64)

std::ifstream fScores("HighScores.txt");

HighScores::HighScores(sf::RenderWindow &window)
{
	if (!hsMenuFont.loadFromFile("fonts/BAUHS93.ttf") || !scoreFont.loadFromFile("fonts/Roboto-Medium.ttf")) {
		//ocupa-te de eroare aici
	}
	else {
		hsMenu[0].setString("Reset Scores");
		hsMenu[0].setFont(hsMenuFont);
		hsMenu[0].setFillColor(customBlack);
		hsMenu[0].setPosition(sf::Vector2f(window.getSize().x - 200, window.getSize().y / 3 * 1));

		hsMenu[1].setString("Main Menu");
		hsMenu[1].setFont(hsMenuFont);
		hsMenu[1].setFillColor(customBlue);
		hsMenu[1].setPosition(sf::Vector2f(window.getSize().x - 200, window.getSize().y / 3 * 1.5));

		selectedItem = 1;
	}



}


HighScores::~HighScores()
{
}

void HighScores::Draw(sf::RenderWindow &window) {
	for (int i = 0; i < 2; i++)
		window.draw(hsMenu[i]);
}

void HighScores::MoveUp() {
	hsMenu[selectedItem].setFillColor(customBlack);
	if (selectedItem == 1) selectedItem = 0;
	else if (selectedItem == 0) selectedItem = 1;
	hsMenu[selectedItem].setFillColor(customBlue);
}
void HighScores::MoveDown() {
	hsMenu[selectedItem].setFillColor(customBlack);
	if (selectedItem == 1) selectedItem = 0;
	else if (selectedItem == 0) selectedItem = 1;
	hsMenu[selectedItem].setFillColor(customBlue);
}
void HighScores::resetScores() {

}