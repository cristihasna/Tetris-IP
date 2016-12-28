#pragma once

#include <SFML\Graphics.hpp>
#include "Pieces.h"
#include "Board.h"
#include <time.h>
class Game
{
public:
	Game(sf::RenderWindow &window);
	~Game();

	void init(Board &Board, Pieces Pieces);
	sf::Vector2i generatePiece(Pieces Pieces);
};

