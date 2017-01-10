#include "Pieces.h"



Pieces::Pieces(sf::RenderWindow &window)
{
}


Pieces::~Pieces()
{
}


bool Pieces::getBlockType(int piece, int rotation, int x, int y) {
	return pieces[piece][rotation][x][y];
}
sf::Vector2i Pieces::getInitialPosition(int piece, int rotation) {
	sf::Vector2i result;
	result.x = Pieces::piecesInitialPosition[piece][rotation][0];
	result.y = Pieces::piecesInitialPosition[piece][rotation][1];
	
	return result;
}