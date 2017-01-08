#pragma once
#include <SFML\Graphics.hpp>
#include <cstring>

#define MAX_H_SCORES 5

struct Score {
	int score;
	float minutesElapsed, secondsElapsed;
};
class HighScores
{
public:
	HighScores(sf::RenderWindow &window);
	~HighScores();
private:
	
	sf::Text hsMenu[2];
	sf::Font scoreFont;
	sf::Font hsMenuFont;

	int selectedItem;

public:
	Score highScores[MAX_H_SCORES];
	sf::Text scores[MAX_H_SCORES * 2];
	void Draw(sf::RenderWindow &window);
	void MoveUp();
	void MoveDown();
	void processScore(int score, float minutesElapsed, float secondsElapsed, Score highScores[MAX_H_SCORES]);
	void readHighScores(Score highScores[MAX_H_SCORES]);
	void transformHighScores(Score highScores[MAX_H_SCORES]);
	void saveScore(Score highScores[MAX_H_SCORES]);
	void resetScores(Score highScores[MAX_H_SCORES]);
	int getSelectedMenuItem() {
		return selectedItem;
	}
};

