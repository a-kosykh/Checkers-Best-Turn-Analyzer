#include "checkers.hpp"

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
	char friendPiece, friendKing, oppoPiece, oppoKing;
	if (_currentNode->getCB().getWhosTurn() == MOVE_RED) {
		friendPiece = PIECE_RED;
		friendKing = PIECE_RED_KING;
		oppoPiece = PIECE_BLACK;
		oppoKing = PIECE_BLACK_KING;
	}
	if (_currentNode->getCB().getWhosTurn() == MOVE_BLACK) {
		friendPiece = PIECE_BLACK;
		friendKing = PIECE_BLACK_KING;
		oppoPiece = PIECE_RED;
		oppoKing = PIECE_RED_KING;
	}

	bool combat = false;
	// UP LEFT
	if (pos % 8 > 1) {
		for (int i = pos - 9; i > 0; i -= 9) {
			if (i % 8 != 0 && i > 8) {
				if (tempBoard.at(i) == oppoPiece ||
					tempBoard.at(i) == oppoKing) {
					if (tempBoard.at(i - 9) == PIECE_NO) {
						combat = true;

						std::vector<int> tempBoard1 = tempBoard;
						std::swap(tempBoard1[pos], tempBoard1[i - 9]);
						tempBoard1[i] = PIECE_NO;

						std::string notation = tempNotation + 'x'
							+ std::to_string(i - 9);

						_moveCombatKing(i - 9, tempBoard1, notation);
						break;
					}
					else break;
				}
				if (tempBoard.at(i) == friendPiece ||
					tempBoard.at(i) == friendKing) {
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
				if (tempBoard.at(i) == oppoPiece ||
					tempBoard.at(i) == oppoKing) {
					if (tempBoard.at(i - 7) == PIECE_NO) {
						combat = true;

						std::vector<int> tempBoard1 = tempBoard;
						std::swap(tempBoard1[pos], tempBoard1[i - 7]);
						tempBoard1[i] = PIECE_NO;

						std::string notation = tempNotation + 'x'
							+ std::to_string(i - 7);

						_moveCombatKing(i - 7, tempBoard1, notation);
						break;
					}
					else break;
				}
				if (tempBoard.at(i) == friendPiece ||
					tempBoard.at(i) == friendKing) {
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
				if (tempBoard.at(i) == oppoPiece ||
					tempBoard.at(i) == oppoKing) {
					if (tempBoard.at(i + 9) == PIECE_NO) {
						combat = true;

						std::vector<int> tempBoard1 = tempBoard;
						std::swap(tempBoard1[pos], tempBoard1[i + 9]);
						tempBoard1[i] = PIECE_NO;

						std::string notation = tempNotation + 'x'
							+ std::to_string(i + 9);

						_moveCombatKing(i + 9, tempBoard1, notation);
						break;
					}
					else break;
				}
				if (_currentNode->getCB().getBoard().at(i) == friendPiece ||
					_currentNode->getCB().getBoard().at(i) == friendKing) {
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
				if (tempBoard.at(i) == oppoPiece ||
					tempBoard.at(i) == oppoKing) {
					if (tempBoard.at(i + 7) == PIECE_NO) {
						combat = true;

						std::vector<int> tempBoard1 = tempBoard;
						std::swap(tempBoard1[pos], tempBoard1[i + 7]);
						tempBoard1[i] = PIECE_NO;

						std::string notation = tempNotation + 'x'
							+ std::to_string(i + 7);

						_moveCombatKing(i + 7, tempBoard1, notation);
						break;
					}
					else break;
				}
				if (tempBoard.at(i) == friendPiece ||
					tempBoard.at(i) == friendKing) {
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
	// CHECK FORWARD MOVE LEFT
	bool combat = false;
	if (pos <= 46) {
		if ((tempBoard[pos + 7] == PIECE_BLACK || tempBoard[pos + 7] == PIECE_BLACK_KING)
			&& tempBoard[pos + 7 + 7] == PIECE_NO && (pos % 8) > 1) {

			combat = true;

			std::vector<int> tempBoard1 = tempBoard;
			std::swap(tempBoard1[pos], tempBoard1[pos + 7 + 7]);
			tempBoard1[pos + 7] = PIECE_NO;

			std::string notation = tempNotation + 'x'
				+ std::to_string(pos + 7 + 7);

			if (pos + 7 + 7 >= 56) {
				tempBoard1[pos + 7 + 7] = PIECE_RED_KING;
				_moveCombatKing(pos + 7 + 7, tempBoard1, notation);
			}
			else {
				_moveCombatRed(pos + 7 + 7, tempBoard1, notation);
			}
		}
	}
	if (pos <= 44) {
		// CHECK FORWARD MOVE RIGHT
		if ((tempBoard[pos + 9] == PIECE_BLACK || tempBoard[pos + 9] == PIECE_BLACK_KING)
			&& tempBoard[pos + 9 + 9] == PIECE_NO && (pos % 8) < 6) {

			combat = true;

			std::vector<int> tempBoard1 = tempBoard;
			std::swap(tempBoard1[pos], tempBoard1[pos + 9 + 9]);
			tempBoard1[pos + 9] = PIECE_NO;

			std::string notation = tempNotation + 'x'
				+ std::to_string(pos + 9 + 9);

			if (pos + 9 + 9 >= 56) {
				tempBoard1[pos + 9 + 9] = PIECE_RED_KING;
				_moveCombatKing(pos + 9 + 9, tempBoard1, notation);
			}
			else {
				_moveCombatRed(pos + 9 + 9, tempBoard1, notation);
			}
		}
	}
	if (pos >= 19) {
		// CHECK BACK MOVE LEFT
		if ((tempBoard[pos - 9] == PIECE_BLACK || tempBoard[pos - 9] == PIECE_BLACK_KING)
			&& tempBoard[pos - 9 - 9] == PIECE_NO && (pos % 8) > 1
			&& (pos > 16)) {

			combat = true;

			std::vector<int> tempBoard1 = tempBoard;
			std::swap(tempBoard1[pos], tempBoard1[pos - 9 - 9]);
			tempBoard1[pos - 9] = PIECE_NO;

			std::string notation = tempNotation + 'x'
				+ std::to_string(pos - 9 - 9);

			_moveCombatRed(pos - 9 - 9, tempBoard1, notation);
		}
	}
	if (pos >= 17) {
		// CHECK BACK MOVE RIGHT
		if ((tempBoard[pos - 7] == PIECE_BLACK || tempBoard[pos - 7] == PIECE_BLACK_KING)
			&& tempBoard[pos - 7 - 7] == PIECE_NO && (pos % 8) < 6
			&& (pos > 16)) {

			combat = true;

			std::vector<int> tempBoard1 = tempBoard;
			std::swap(tempBoard1[pos], tempBoard1[pos - 7 - 7]);
			tempBoard1[pos - 7] = PIECE_NO;

			std::string notation = tempNotation + 'x'
				+ std::to_string(pos - 7 - 7);

			_moveCombatRed(pos - 7 - 7, tempBoard1, notation);
		}
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
	if (pos >= 19) {
		// CHECK FORWARD MOVE LEFT
		if ((tempBoard[pos - 9] == PIECE_RED || tempBoard[pos - 9] == PIECE_RED_KING)
			&& tempBoard[pos - 9 - 9] == PIECE_NO && (pos % 8) > 1
			&& (pos > 16)) {

			combat = true;

			std::vector<int> tempBoard1 = tempBoard;
			std::swap(tempBoard1[pos], tempBoard1[pos - 9 - 9]);
			tempBoard1[pos - 9] = PIECE_NO;

			std::string notation = tempNotation + 'x'
				+ std::to_string(pos - 9 - 9);

			if (pos - 9 - 9 <= 7) {
				tempBoard1[pos - 9 - 9] = PIECE_BLACK_KING;
				_moveCombatKing(pos - 9 - 9, tempBoard1, notation);
			}
			else {
				_moveCombatRed(pos - 9 - 9, tempBoard1, notation);
			}
		}
	}
	if (pos >= 17) {
		// CHECK FORWARD MOVE RIGHT
		if ((tempBoard[pos - 7] == PIECE_RED || tempBoard[pos - 7] == PIECE_RED_KING)
			&& tempBoard[pos - 7 - 7] == PIECE_NO && (pos % 8) < 6
			&& (pos > 16)) {

			combat = true;

			std::vector<int> tempBoard1 = tempBoard;
			std::swap(tempBoard1[pos], tempBoard1[pos - 7 - 7]);
			tempBoard1[pos - 7] = PIECE_NO;

			std::string notation = tempNotation + 'x'
				+ std::to_string(pos - 7 - 7);

			if (pos - 7 - 7 <= 7) {
				tempBoard1[pos - 7 - 7] = PIECE_BLACK_KING;
				_moveCombatKing(pos - 7 - 7, tempBoard1, notation);
			}
			else {
				_moveCombatRed(pos - 7 - 7, tempBoard1, notation);
			}
		}
	}
	if (pos <= 46) {
		// CHECK BACK MOVE LEFT
		if ((tempBoard[pos + 7] == PIECE_RED || tempBoard[pos + 7] == PIECE_RED_KING)
			&& tempBoard[pos + 7 + 7] == PIECE_NO && (pos % 8) > 1) {

			combat = true;

			std::vector<int> tempBoard1 = tempBoard;
			std::swap(tempBoard1[pos], tempBoard1[pos + 7 + 7]);
			tempBoard1[pos + 7] = PIECE_NO;

			std::string notation = tempNotation + 'x'
				+ std::to_string(pos + 7 + 7);

			_moveCombatBlack(pos + 7 + 7, tempBoard1, notation);
		}
	}
	if (pos <= 44) {
		// CHECK BACK MOVE RIGHT
		if ((tempBoard[pos + 9] == PIECE_RED || tempBoard[pos + 9] == PIECE_RED_KING)
			&& tempBoard[pos + 9 + 9] == PIECE_NO && (pos % 8) < 6) {

			combat = true;

			std::vector<int> tempBoard1 = tempBoard;
			std::swap(tempBoard1[pos], tempBoard1[pos + 9 + 9]);
			tempBoard1[pos + 9] = PIECE_NO;

			std::string notation = tempNotation + 'x'
				+ std::to_string(pos + 9 + 9);

			_moveCombatBlack(pos + 9 + 9, tempBoard1, notation);
		}
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
		- (std::count(board.begin(), board.end(), PIECE_BLACK) + (KING_COEF)*(std::count(board.begin(), board.end(), PIECE_BLACK)));
	if (node->getCB().getWhosTurn() == MOVE_BLACK)
		++_val;
	if (node->getCB().getWhosTurn() == MOVE_RED)
		--_val;

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
	int friendPiece = PIECE_RED;
	int friendKing = PIECE_RED_KING;
	if (node->getCB().getWhosTurn() == MOVE_BLACK) {
		friendPiece = PIECE_BLACK;
		friendKing = PIECE_BLACK_KING;
	}
	double score = 0;
	for (unsigned int i = 0; i < node->getCB().getBoard().size(); ++i) {
		if (node->getCB().getBoard().at(i) == friendPiece ||
			node->getCB().getBoard().at(i) == friendKing) {
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
	TreeNode* bestNode = nullptr;
	std::vector<TreeNode*> bestMoves;
	for (unsigned int i = 0; i < root->getChildren().size(); ++i) {
		if (root->getChildren().at(i)->getValue() == bestScore)
			bestMoves.push_back(root->getChildren().at(i));
	}
	for (auto i = bestMoves.begin(); i != bestMoves.end(); ++i) {
		evaluateExtraScore(*i);
	}
	for (unsigned int i = 0; i < bestMoves.size(); ++i) {
		if (bestScore < bestMoves.at(i)->getExtraScore()) {
			bestScore = bestMoves.at(i)->getExtraScore();
			bestNode = bestMoves.at(i);
		}
	}

	return bestNode->getNotation();
}
