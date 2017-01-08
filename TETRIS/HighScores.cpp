#include "HighScores.h"
#include <fstream>
#include <iostream>

#define customBlue sf::Color (134, 181, 225)
#define customBlack sf::Color (55, 65, 64)

std::ifstream fScores("HighScores.txt");

HighScores::HighScores(sf::RenderWindow &window)
{
	if (!scoreFont.loadFromFile("fonts/Roboto-Medium.ttf")) {
		//ocupa-te de eroare aici;
	}
	else if (!hsMenuFont.loadFromFile("fonts/BAUHS93.ttf") || !scoreFont.loadFromFile("fonts/Roboto-Medium.ttf")) {
		//ocupa-te de eroare aici
	}
	else {
		hsMenu[0].setString("Reset Scores");
		hsMenu[0].setFont(hsMenuFont);
		hsMenu[0].setFillColor(customBlack);
		hsMenu[0].setPosition(sf::Vector2f(50, window.getSize().y/2 -50));

		hsMenu[1].setString("Main Menu");
		hsMenu[1].setFont(hsMenuFont);
		hsMenu[1].setFillColor(customBlue);
		hsMenu[1].setPosition(sf::Vector2f(50, window.getSize().y/2));

		selectedItem = 1;
	}
}


HighScores::~HighScores()
{
}

void HighScores::Draw(sf::RenderWindow &window) {
	sf::Texture backgroundTexture;
	backgroundTexture.loadFromFile("images/hsBg.jpg");
	sf::Sprite background(backgroundTexture);
	background.setTextureRect(sf::IntRect(0, 0, window.getSize().x, window.getSize().y));

	window.draw(background);


	for (int i = 0; i < 10; i++) {
		//std::cout << HighScores::scores[i].getPosition().y << std::endl;
		window.draw(HighScores::scores[i]);

	}

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

void HighScores::resetScores( Score highScores[MAX_H_SCORES]) {
	for (int i = 0; i < MAX_H_SCORES; i++) {
		highScores[i].score = 0;
		highScores[i].minutesElapsed = 0;
		highScores[i].secondsElapsed = 0;
	}
	HighScores::saveScore(highScores);
}

void HighScores::saveScore(Score highScores[MAX_H_SCORES]) {
	std::ofstream o("HighScores.txt");
	for (int i = 0; i < MAX_H_SCORES; i++) {
		o << highScores[i].score << " ";
		o << highScores[i].minutesElapsed << " ";
		o << highScores[i].secondsElapsed;
		if (i != MAX_H_SCORES - 1) o << std::endl;
	}
}

void HighScores::processScore(int score, float minutesElapsed, float secondsElapsed, Score highScores[MAX_H_SCORES]) {
	HighScores::readHighScores(highScores);
	if (score > highScores[MAX_H_SCORES - 1].score) {
		std::cout << "procesez scorul " <<score<< std::endl;
		highScores[MAX_H_SCORES - 1].score = score;
		highScores[MAX_H_SCORES - 1].minutesElapsed = minutesElapsed;
		highScores[MAX_H_SCORES - 1].secondsElapsed = (int)secondsElapsed;

		for (int i = MAX_H_SCORES - 1; i > 0; i--) {
			if (highScores[i].score > highScores[i - 1].score) {
				Score auxiliar = highScores[i];
				highScores[i] = highScores[i - 1];
				highScores[i - 1] = auxiliar;
			}
		}
		HighScores::saveScore(highScores);
	}
	std::cout<<"scorul dupa procesare:"<<std::endl;
	for (int i = 0; i < MAX_H_SCORES; i++) {
		std::cout << highScores[i].score << " ";
		std::cout << highScores[i].minutesElapsed << " ";
		std::cout << highScores[i].secondsElapsed << std::endl;
	}

}

void HighScores::readHighScores(Score highScores[MAX_H_SCORES]) {
	
	std::ifstream f("HighScores.txt");
	std::cout << "citesc din fisier: " << std::endl;
	for (int i = 0; i < MAX_H_SCORES; i++) {
		f >> highScores[i].score;
		std::cout << highScores[i].score << " ";
		f >> highScores[i].minutesElapsed;
		std::cout << highScores[i].minutesElapsed << " ";
		f >> highScores[i].secondsElapsed;
		std::cout << highScores[i].secondsElapsed << std::endl;
	}
	HighScores::transformHighScores(highScores);
}

void HighScores::transformHighScores(Score highScores[MAX_H_SCORES]) {
	std::string auxiliar = "";

	for (int i = 0; i < 10; i++) {
		HighScores::scores[i].setFont(scoreFont);
		HighScores::scores[i].setFillColor(sf::Color::White);
		if (i % 2 == 0) {
			if(highScores[i/2].score>0)
				HighScores::scores[i].setString(std::to_string(highScores[i / 2].score));
			else
				HighScores::scores[i].setString("-");
			HighScores::scores[i].setPosition(sf::Vector2f(335, 103 + i / 2 * 60));
			HighScores::scores[i].setCharacterSize(35);
		}
		else {
			HighScores::scores[i].setPosition(sf::Vector2f(470, 115 + i / 2 * 60));
			HighScores::scores[i].setCharacterSize(13);
			if (highScores[i / 2].score <= 0)
				auxiliar = "- - : - -";
			else {
				if (highScores[i / 2].minutesElapsed <= 9)
					auxiliar = "0" + std::to_string((int)highScores[i / 2].minutesElapsed) + ":";
				else
					auxiliar = std::to_string((int)highScores[i / 2].minutesElapsed) + ":";

				if (highScores[i / 2].secondsElapsed <= 9)
					auxiliar = auxiliar + "0" + std::to_string((int)highScores[i / 2].secondsElapsed);
				else
					auxiliar = auxiliar + std::to_string((int)highScores[i / 2].secondsElapsed);
			}

			

			HighScores::scores[i].setString(auxiliar);
		}
	}
}