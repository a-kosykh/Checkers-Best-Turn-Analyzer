#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <limits>

#define BOARD_ROWS 8
#define BOARD_COLUMNS 8

#define MOVE_RED 0
#define MOVE_BLACK 1

#define PIECE_NO 0
#define PIECE_RED 1
#define PIECE_BLACK 2
#define PIECE_RED_KING 3
#define PIECE_BLACK_KING 4

// победа
enum class Win {
	winNo, // нет победы
	winRed,
	winBlack
};

#define WIN_VALUE 100 // Очки победы

#define KING_COEF 2 // коэфициент королевской шашки

// ошибки 
enum class InputError {
	noError,
	badSize,
	badDepth,
	badStartTurn,
	badCharOnBoard,
	emptyBoard
};
