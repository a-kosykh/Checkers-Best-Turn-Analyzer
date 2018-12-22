#pragma once
#include "defines.hpp"
#include <vector>
#include <iostream>

class CheckersBoard {
	std::vector<int>			_board; // доска в виде строки
	bool 						_whosTurn; // флаг "чей ход" (красных или черных)

public:
	CheckersBoard() {}
	CheckersBoard(const std::vector<int> &positions, bool move) : _board(positions), _whosTurn(move) {}
	
	// геттеры полей
	std::vector<int> getBoard() const;
	bool getWhosTurn() const;
};
