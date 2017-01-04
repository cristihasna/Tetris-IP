#pragma once
#include <SFML\Graphics.hpp>


#define MAX_H_SCORES 5
class HighScores
{
public:
	HighScores(sf::RenderWindow &window);
	~HighScores();
private:
	sf::Text scores[MAX_H_SCORES];
	sf::Text hsMenu[2];
	sf::Font scoreFont;
	sf::Font hsMenuFont;

	int selectedItem;

public:

	void Draw(sf::RenderWindow &window);
	void MoveUp();
	void MoveDown();


	int getSelectedMenuItem() {
		return selectedItem;
	}
	void resetScores();

};

