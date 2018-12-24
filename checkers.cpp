#include "checkers.hpp"

int Checkers::_pieceOpponent(bool king, TreeNode* node)
{
	if (node->getCB().getWhosTurn() == MOVE_RED) {
		if (!king) {
			return PIECE_BLACK;
		}
		else return PIECE_BLACK_KING;
	}
	else {
		if (!king) {
			return PIECE_RED;
		}
		else return PIECE_RED_KING;
	}
}

int Checkers::_pieceFriend(bool king, TreeNode* node)
{
	if (node->getCB().getWhosTurn() == MOVE_RED) {
		if (!king) {
			return PIECE_RED;
		}
		else return PIECE_RED_KING;
	}
	else {
		if (!king) {
			return PIECE_BLACK;
		}
		else return PIECE_BLACK_KING;
	}
}

bool Checkers::_isLegaltoKing(int finalPos)
{
	if (_currentNode->getCB().getWhosTurn() == MOVE_RED) {
		if (finalPos >= 56) {
			return true;
		}
	}
	if (_currentNode->getCB().getWhosTurn() == MOVE_BLACK) {
		if (finalPos <= 7) {
			return true;
		}
	}
	return false;
}

bool Checkers::_isLegaltoCombatUpLeft(std::vector<int> tempBoard, int pos)
{	
	if (pos >= 19) {
		if ((tempBoard[pos - 9] == _pieceOpponent(false, _currentNode) || tempBoard[pos - 9] == _pieceOpponent(true, _currentNode))
			&& tempBoard[pos - 9 - 9] == PIECE_NO && (pos % 8) > 1
			&& (pos > 16))
			return true;
	}
	return false;
}

bool Checkers::_isLegaltoCombatUpRight(std::vector<int> tempBoard, int pos)
{
	if (pos >= 17) {
		if ((tempBoard[pos - 7] == _pieceOpponent(false, _currentNode) || tempBoard[pos - 7] == _pieceOpponent(true, _currentNode))
			&& tempBoard[pos - 7 - 7] == PIECE_NO && (pos % 8) < 6
			&& (pos > 16))
			return true;
	}
	return false;
}

bool Checkers::_isLegaltoCombatDownRight(std::vector<int> tempBoard, int pos)
{
	if (pos <= 44) {
		if ((tempBoard[pos + 9] == _pieceOpponent(false, _currentNode) || tempBoard[pos + 9] == _pieceOpponent(true, _currentNode))
			&& tempBoard[pos + 9 + 9] == PIECE_NO && (pos % 8) < 6)
			return true;
	}
	return false;
}

bool Checkers::_isLegaltoCombatDownLeft(std::vector<int> tempBoard, int pos)
{
	if (pos <= 46) {
		if ((tempBoard[pos + 7] == _pieceOpponent(false, _currentNode) || tempBoard[pos + 7] == _pieceOpponent(true, _currentNode))
			&& tempBoard[pos + 7 + 7] == PIECE_NO && (pos % 8) > 1)
			return true;
	}
	return false;
}

std::vector<int> Checkers::_swapCombatPieces(std::vector<int> tempBoard, int startPos, int nullPos,
	int finalPos, bool toKing)
{
	std::swap(tempBoard[startPos], tempBoard[finalPos]);
	tempBoard[nullPos] = PIECE_NO;
	if (toKing) {
		tempBoard[finalPos] = _pieceFriend(true, _currentNode);
	}
	return tempBoard;
}

std::string Checkers::_createNotation(std::string notation, int finalPos, bool isCombat)
{
	char mark = '-';
	if (isCombat)
		mark = 'x';
	notation = notation + mark + std::to_string(finalPos);
	return notation;
}

void Checkers::_createNoFightNode(int startPos, int finalPos)
{
	std::vector<int> tempBoard = _currentNode->getCB().getBoard();

	if (_isLegaltoKing(finalPos) && tempBoard[startPos] == PIECE_RED) {
		tempBoard[startPos] = PIECE_RED_KING;
	}
	if (_isLegaltoKing(finalPos) && tempBoard[startPos] == PIECE_BLACK) {
		tempBoard[startPos] = PIECE_BLACK_KING;
	}
	std::swap(tempBoard[startPos], tempBoard[finalPos]);

	std::string notation = std::to_string(startPos) + "-" + std::to_string(finalPos);

	_currentNode->addChild(new TreeNode(CheckersBoard(tempBoard, !_currentNode->getCB().getWhosTurn()), notation));
	_nodeQ.push(_currentNode->getChildren().back());
}

void Checkers::_moveNoFightKing(int pos)
{
	// move king up left
	if (pos % 8 != 0) {
		for (int i = pos - 9; (i >= 0); i -= 9) {
			if (_currentNode->getCB().getBoard().at(i) == PIECE_NO) {
				_createNoFightNode(pos, i);
				if (i % 8 == 0 || i < 8)
					break;
			}
			else break;
		}
	}
	// move king up right
	if (pos % 8 != 7) {
		for (int i = pos - 7; (i >= 0); i -= 7) {
			if (_currentNode->getCB().getBoard().at(i) == PIECE_NO) {
				_createNoFightNode(pos, i);
				if (i % 8 == 7 || i < 8)
					break;
			}
			else break;
		}
	}
	// move king down right
	if (pos % 8 != 7) {
		for (int i = pos + 9; (i <= 63); i += 9) {
			if (_currentNode->getCB().getBoard().at(i) == PIECE_NO) {
				_createNoFightNode(pos, i);
				if (i % 8 == 7 || i > 55)
					break;
			}
			else break;
		}
	}
	// move king down left
	if (pos % 8 != 0) {
		for (int i = pos + 7; (i <= 63); i += 7) {
			if (_currentNode->getCB().getBoard().at(i) == PIECE_NO) {
				_createNoFightNode(pos, i);
				if (i % 8 == 0 || i > 55)
					break;
			}
			else break;
		}
	}
}

void Checkers::_moveNoFightRed(int pos)
{
	// SIMPLE MOVE LEFT
	if (pos % 8 != 0) {
		if (_currentNode->getCB().getBoard().at(pos + 7) == PIECE_NO) {
			_createNoFightNode(pos, pos + 7);
		}
	}
	// SIMPLE MOVE RIGHT
	if (pos % 8 != 7) {
		if (_currentNode->getCB().getBoard().at(pos + 9) == PIECE_NO) {
			_createNoFightNode(pos, pos + 9);
		}
	}
}

void Checkers::_moveNoFightBlack(int pos)
{
	if (pos % 8 != 7) {
		if (_currentNode->getCB().getBoard().at(pos - 7) == PIECE_NO) {
			_createNoFightNode(pos, pos - 7);
		}
	}
	if (pos % 8 != 0) {
		if (_currentNode->getCB().getBoard().at(pos - 9) == PIECE_NO) {
			_createNoFightNode(pos, pos - 9);
		}
	}
}

void Checkers::_moveCombatKing(int pos, std::vector<int> tempBoard, std::string tempNotation)
{
	bool combat = false;
	// UP LEFT
	if (pos % 8 > 1) {
		for (int i = pos - 9; i > 0; i -= 9) {
			if (i % 8 != 0 && i > 8) {
				if (tempBoard.at(i) == _pieceOpponent(false, _currentNode) ||
					tempBoard.at(i) == _pieceOpponent(true, _currentNode)) {
					if (tempBoard.at(i - 9) == PIECE_NO) {
						int finalPos = i - 9;
						combat = true;

						_moveCombatKing(
							finalPos, 
							_swapCombatPieces(tempBoard,pos, i, finalPos,false), 
							_createNotation(tempNotation,finalPos,true));

						break;
					}
					else break;
				}
				if (tempBoard.at(i) == _pieceFriend(false, _currentNode) ||
					tempBoard.at(i) == _pieceFriend(true, _currentNode)) {
					break;
				}
			}
			else break;
		}
	}
	// UP RIGHT
	if (pos % 8 < 7) {
		for (int i = pos - 7; i > 0; i -= 7) {
			if (i % 8 != 7 && i > 8) {
				if (tempBoard.at(i) == _pieceOpponent(false, _currentNode) ||
					tempBoard.at(i) == _pieceOpponent(true, _currentNode)) {
					if (tempBoard.at(i - 7) == PIECE_NO) {
						int finalPos = i - 7;
						combat = true;

						_moveCombatKing(
							finalPos,
							_swapCombatPieces(tempBoard, pos, i, finalPos, false),
							_createNotation(tempNotation, finalPos, true));

						break;
					}
					else break;
				}
				if (tempBoard.at(i) == _pieceFriend(false, _currentNode) ||
					tempBoard.at(i) == _pieceFriend(true, _currentNode)) {
					break;
				}
			}
			else break;
		}
	}
	// DOWN RIGHT
	if (pos % 8 < 7) {
		for (int i = pos + 9; i < 63; i += 9) {
			if (i % 8 != 7 && i < 55) {
				if (tempBoard.at(i) == _pieceOpponent(false, _currentNode) ||
					tempBoard.at(i) == _pieceOpponent(true, _currentNode)) {
					if (tempBoard.at(i + 9) == PIECE_NO) {
						int finalPos = i + 9;
						combat = true;

						_moveCombatKing(
							finalPos,
							_swapCombatPieces(tempBoard, pos, i, finalPos, false),
							_createNotation(tempNotation, finalPos, true));

						break;
					}
					else break;
				}
				if (tempBoard.at(i) == _pieceFriend(false, _currentNode) ||
					tempBoard.at(i) == _pieceFriend(true, _currentNode)) {
					break;
				}
			}
			else break;
		}
	}
	// DOWN LEFT
	if (pos % 8 > 1) {
		for (int i = pos + 7; i < 63; i += 7) {
			if (i % 8 != 0 && i < 55) {
				if (tempBoard.at(i) == _pieceOpponent(false, _currentNode) ||
					tempBoard.at(i) == _pieceOpponent(true , _currentNode)) {
					if (tempBoard.at(i + 7) == PIECE_NO) {
						int finalPos = i + 7;
						combat = true;

						_moveCombatKing(
							finalPos,
							_swapCombatPieces(tempBoard, pos, i, finalPos, false),
							_createNotation(tempNotation, finalPos, true));

						break;
					}
					else break;
				}
				if (tempBoard.at(i) == _pieceFriend(false, _currentNode) ||
					tempBoard.at(i) == _pieceFriend(true, _currentNode)) {
					break;
				}
			}
			else break;
		}
	}


	// END RECURSION
	if (tempBoard != _currentNode->getCB().getBoard() && combat == false) {
		_currentNode->addChild(new TreeNode(CheckersBoard(tempBoard, !_currentNode->getCB().getWhosTurn()), tempNotation));
		_nodeQ.push(_currentNode->getChildren().back());
		return;
	}
}

void Checkers::_moveCombatRed(int pos, std::vector<int> tempBoard, std::string tempNotation)
{
	bool combat = false;
	
	// DOWN LEFT
	if (_isLegaltoCombatDownLeft(tempBoard, pos)) {
		int finalPos = pos + 7 + 7; // DOWNLEFT
		
		combat = true;

		if (finalPos >= 56) {
			_moveCombatKing(
				finalPos, 
				_swapCombatPieces(tempBoard, pos, pos + 7, finalPos, true), 
				_createNotation(tempNotation, finalPos, true));
		}
		else {
			_moveCombatRed(
				finalPos, 
				_swapCombatPieces(tempBoard, pos, pos + 7, finalPos, false), 
				_createNotation(tempNotation, finalPos, true));
		}
	}
	// DOWN RIGHT
	if (_isLegaltoCombatDownRight(tempBoard,pos)){
		int finalPos = pos + 9 + 9;
		combat = true;

		if (finalPos >= 56) {
			_moveCombatKing(
				finalPos, 
				_swapCombatPieces(tempBoard,pos, pos + 9,finalPos,true), 
				_createNotation(tempNotation, finalPos, true));
		}
		else {
			_moveCombatRed(
				finalPos,
				_swapCombatPieces(tempBoard, pos, pos + 9, finalPos, false),
				_createNotation(tempNotation, finalPos, true));
		}
	}
	// UP LEFT
	if (_isLegaltoCombatUpLeft(tempBoard, pos)) {
		int finalPos = pos - 9 - 9;
		combat = true;

		_moveCombatRed(
			finalPos, 
			_swapCombatPieces(tempBoard,pos, pos - 9, finalPos, false),
			_createNotation(tempNotation,finalPos,true));
	}
	// UP RIGHT
	if (_isLegaltoCombatUpRight(tempBoard, pos)) {
		int finalPos = pos - 7 - 7;
		combat = true;

		_moveCombatRed(
			finalPos, 
			_swapCombatPieces(tempBoard, pos, pos - 7, finalPos, false),
			_createNotation(tempNotation, finalPos, true));
	}
	// END RECURSION
	if (tempBoard != _currentNode->getCB().getBoard() && combat == false) {
		_currentNode->addChild(new TreeNode(CheckersBoard(tempBoard, true), tempNotation));
		_nodeQ.push(_currentNode->getChildren().back());
		return;
	}
}

void Checkers::_moveCombatBlack(int pos, std::vector<int> tempBoard, std::string tempNotation)
// условия и создание дочернего узла при бое черной шашки
{
	bool combat = false;
	// UP LEFT
	if (_isLegaltoCombatUpLeft(tempBoard, pos)){
		int finalPos = pos - 9 - 9;
		combat = true;

		if (finalPos <= 7) {
			_moveCombatKing(
				finalPos,
				_swapCombatPieces(tempBoard, pos, pos - 9, finalPos, true),
				_createNotation(tempNotation, finalPos, true));
			}
		else {
			_moveCombatBlack(
				finalPos,
				_swapCombatPieces(tempBoard, pos, pos - 9, finalPos, false),
				_createNotation(tempNotation, finalPos, true));
		}
	}
	// UP RIGHT
	if (_isLegaltoCombatUpRight(tempBoard, pos)) {
		int finalPos = pos - 7 - 7;
		combat = true;

		if (finalPos <= 7) {
				_moveCombatKing(
					finalPos,
					_swapCombatPieces(tempBoard, pos, pos - 7 , finalPos, true),
					_createNotation(tempNotation, finalPos, true));
		}
		else {
				_moveCombatBlack(
					finalPos,
					_swapCombatPieces(tempBoard, pos, pos - 7, finalPos, false),
					_createNotation(tempNotation, finalPos, true));
		}
	}
	// DOWN LEFT
	if (_isLegaltoCombatDownLeft(tempBoard, pos)) {
		int finalPos = pos + 7 + 7;
		combat = true;

		_moveCombatBlack(
			finalPos,
			_swapCombatPieces(tempBoard, pos, pos + 7, finalPos, false),
			_createNotation(tempNotation, finalPos, true));
	}
	// DOWN RIGHT
	if (_isLegaltoCombatDownRight(tempBoard, pos)) {
		int finalPos = pos + 9 + 9;
		combat = true;

		_moveCombatBlack(
			finalPos,
			_swapCombatPieces(tempBoard, pos,pos + 9, finalPos, false),
			_createNotation(tempNotation, finalPos, true));
	}
	// END RECURSION
	if (tempBoard != _currentNode->getCB().getBoard() && combat == false) {
		_currentNode->addChild(new TreeNode(CheckersBoard(tempBoard, false), tempNotation));
		_nodeQ.push(_currentNode->getChildren().back());
		return;
	}
}

void Checkers::_makeMove()
// попытка сделать ход
{
	// для каждой клетки текущего узла 
	for (unsigned int i = 0; i < _currentNode->getCB().getBoard().size() - 1; ++i) {
		// если ход красных
		if ((_currentNode->getCB().getBoard().at(i) == PIECE_RED || _currentNode->getCB().getBoard().at(i) == PIECE_RED_KING)
			&& _currentNode->getCB().getWhosTurn() == MOVE_RED) {

			auto currChildSize = _currentNode->getChildren().size();
			// Проверка возможности вступить в бой если обычная шашка
			if (_currentNode->getCB().getBoard().at(i) == PIECE_RED) {
				_moveCombatRed(i, _currentNode->getCB().getBoard(), std::to_string(i));
			}
			// Проверка возможности вступить в бой если король
			if (_currentNode->getCB().getBoard().at(i) == PIECE_RED_KING) {
				_moveCombatKing(i, _currentNode->getCB().getBoard(), std::to_string(i));
			}
			// если боя обычной шашки не было (очередь не увеличилась), то попытка тихого хода
			if (_currentNode->getChildren().size() == currChildSize &&
				(i < 56) && _currentNode->getCB().getBoard().at(i) == PIECE_RED)
			{
				_moveNoFightRed(i);
			}
			// если боя короля не было (очередь не увеличилась), то попытка тихого хода
			if (_currentNode->getChildren().size() == currChildSize &&
				_currentNode->getCB().getBoard().at(i) == PIECE_RED_KING)
			{
				_moveNoFightKing(i);
			}
		}
		// если ход черных
		if ((_currentNode->getCB().getBoard().at(i) == PIECE_BLACK || _currentNode->getCB().getBoard().at(i) == PIECE_BLACK_KING) &&
			_currentNode->getCB().getWhosTurn() == MOVE_BLACK) {

			auto currChildSize = _currentNode->getChildren().size();
			// Проверка возможности вступить в бой если обычная шашка
			if (_currentNode->getCB().getBoard().at(i) == PIECE_BLACK) {
				_moveCombatBlack(i, _currentNode->getCB().getBoard(), std::to_string(i));
			}
			// Проверка возможности вступить в бой если король
			if (_currentNode->getCB().getBoard().at(i) == PIECE_BLACK_KING) {
				_moveCombatKing(i, _currentNode->getCB().getBoard(), std::to_string(i));
			}
			// если боя обычной шашки не было (очередь не увеличилась), то попытка тихого хода
			if (_currentNode->getChildren().size() == currChildSize &&
				(i > 7) && _currentNode->getCB().getBoard().at(i) == PIECE_BLACK)
			{
				_moveNoFightBlack(i);
			}
			// если боя короля не было (очередь не увеличилась), то попытка тихого хода
			if (_currentNode->getChildren().size() == currChildSize &&
				_currentNode->getCB().getBoard().at(i) == PIECE_BLACK_KING) {
				_moveNoFightKing(i);
			}
		}
		else continue;
	}
	// если у не было возможности хода (тупик), победа другой стороны
	if (_currentNode->getChildren().size() == 0) {
		if (_currentNode->getCB().getWhosTurn() == MOVE_RED) {
			_currentNode->setWin((int)Win::winBlack);
			_currentNode->setValue(-WIN_VALUE);
		}
		if (_currentNode->getCB().getWhosTurn() == MOVE_BLACK) {
			_currentNode->setWin((int)Win::winRed);
			_currentNode->setValue(WIN_VALUE);
		}
	}
}

void Checkers::_recursiveBuildTree(unsigned int currDepth, unsigned int depth)
// рекурсивное построение дерева
{
	// увеличение текущей глубины
	currDepth++;
	// если текущая глубина меньше целевой глубины, делаем ходы для всех узлов, находящихся в очереди
	// узел добавляется в очередь как только он создаётся
	if (currDepth < depth) {
		unsigned int staticQSize = _nodeQ.size();
		for (unsigned int i = 0; i < staticQSize; ++i) {
			_currentNode = _nodeQ.front();
			if (_currentNode->getWin() == (int)Win::winNo) {
				_makeMove();
			}
			_nodeQ.pop();
		}
		_recursiveBuildTree(currDepth, depth);
	}
	else {
		return;
	}
}

double Checkers::_evaluateValue(TreeNode *& node)
// функция расчета очков
{
	if (node->getWin() == (int)Win::winRed ||
		node->getWin() == (int)Win::winBlack)
		return node->getValue();

	std::vector<int> board = node->getCB().getBoard();
	double _val = (std::count(board.begin(), board.end(), PIECE_RED) + KING_COEF*(std::count(board.begin(), board.end(), PIECE_RED_KING)))
		- (std::count(board.begin(), board.end(), PIECE_BLACK) + (KING_COEF)*(std::count(board.begin(), board.end(), PIECE_BLACK_KING)));

	return _val;
}

double Checkers::_maxValue(TreeNode *& node, double alpha, double beta)
{
	{
		if (node->getChildren().size() == 0) {
			// на последнем слое считаем количество очков
			node->setValue(_evaluateValue(node));
			return node->getValue();
		}
		node->setValue(-std::numeric_limits<double>::infinity());
		for (unsigned int i = 0; i < node->getChildren().size(); ++i) {
			double tempValue = _minValue(
				node->getChildren().at(i), node->getAlpha(), node->getBeta());
			if (tempValue > node->getValue())
				node->setValue(tempValue);
			if (tempValue >= node->getBeta())
				return node->getValue();
			if (tempValue > node->getAlpha())
				node->setAlpha(tempValue);
		}
		return node->getValue();
	}
}

double Checkers::_minValue(TreeNode *& node, double alpha, double beta)
{
	if (node->getChildren().size() == 0) {
		// на последнем слое считаем количество очков
		node->setValue(_evaluateValue(node));
		return node->getValue();
	}
	node->setValue(std::numeric_limits<double>::infinity());
	for (unsigned int i = 0; i < node->getChildren().size(); ++i) {
		double tempValue = _maxValue(
			node->getChildren().at(i), node->getAlpha(), node->getBeta());
		if (tempValue < node->getValue())
			node->setValue(tempValue);
		if (tempValue <= node->getAlpha())
			return node->getValue();
		if (tempValue < node->getBeta())
			node->setBeta(tempValue);
	}
	return node->getValue();
}

Checkers::Checkers(TreeNode * rootNode)
{
	_currentNode = rootNode;
}

void Checkers::buildTree(int depth)
// начало построения дерева 
{
	if (_currentNode->getParent() == nullptr) {
		if (_currentNode->getWin() == (int)Win::winNo) {
			_makeMove();
			_recursiveBuildTree(0, depth);
		}
	}
}

double Checkers::evaluateBestMove(TreeNode *& root)
// запуск функции расчета лучшего балла
{
	if (root->getCB().getWhosTurn() == MOVE_RED)
		return _maxValue(root, root->getAlpha(), root->getBeta());
	if (root->getCB().getWhosTurn() == MOVE_BLACK) {
		return _minValue(root, root->getAlpha(), root->getBeta());
	}
}

void Checkers::evaluateExtraScore(TreeNode *& node)
{
	double score = 0;
	for (unsigned int i = 0; i < node->getCB().getBoard().size(); ++i) {
		if (node->getCB().getBoard().at(i) == _pieceOpponent(false, node) ||
			node->getCB().getBoard().at(i) == _pieceOpponent(true, node)) {
			if ((i % 8) <= 3)
				score += 4 - (i % 8);
			else score += (i % 8) - 4;
		}
	}
	node->setExtraScore(score);
}

std::string Checkers::getBestScoreNotation(TreeNode *& root)
// получение нотации лучшего хода 
{
	if (root->getWin() == 1) {
		return "red win";
	}
	if (root->getWin() == 2) {
		return "black win";
	}
	double bestScore = 0;
	if (root->getCB().getWhosTurn() == MOVE_RED)
		bestScore = _maxValue(root, root->getAlpha(), root->getBeta());
	if (root->getCB().getWhosTurn() == MOVE_BLACK) {
		bestScore = _minValue(root, root->getAlpha(), root->getBeta());
	}
	// так как алгоритм "альфа-бета отсечения" устанавливает value одинаковым значением
	// на всех пути от корня до листа,
	// необходимо взять узел из детей корня с таким же значением value
	// если оценки некоторых совпадают, запускаем дополнительное вычисление по расстоянию от боковой границы
	TreeNode* bestNode = nullptr;
	std::vector<TreeNode*> bestMoves;
	for (unsigned int i = 0; i < root->getChildren().size(); ++i) {
		if (root->getChildren().at(i)->getValue() == bestScore)
			bestMoves.push_back(root->getChildren().at(i));
	}
	if (bestMoves.size() > 1) {
		for (auto i = bestMoves.begin(); i != bestMoves.end(); ++i) {
			evaluateExtraScore(*i);
		}

		bestScore = bestMoves.at(0)->getExtraScore();
		bestNode = bestMoves.at(0);
		for (unsigned int i = 1; i < bestMoves.size(); ++i) {
			if (bestScore < bestMoves.at(i)->getExtraScore()) {
				bestScore = bestMoves.at(i)->getExtraScore();
				bestNode = bestMoves.at(i);
			}
		}
	}
	else bestNode = bestMoves.at(0);

	return bestNode->getNotation();
}
