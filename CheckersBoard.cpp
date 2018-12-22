#include "CheckersBoard.hpp"

std::vector<int> CheckersBoard::getBoard() const
{
	return _board;
}

bool CheckersBoard::getWhosTurn() const
{
	return _whosTurn;
}
