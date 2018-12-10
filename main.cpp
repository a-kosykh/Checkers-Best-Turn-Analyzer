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

// чтение входных данных
std::string getStringStartPos(const std::string &filepath) {
	std::string pos;

	std::ifstream posFile(filepath);
	if (posFile.is_open()) {
		while (!posFile.eof()) {
			std::string temp;
			posFile >> temp;
			pos += temp;
		}
		posFile.close();
	}


	return pos;
}

// шашечная доска
class CheckersBoard {
	std::string 				_board; // доска в виде строки
	bool 						_whosTurn; // флаг "чей ход" (красных или черных)

public:
	CheckersBoard() {}
	CheckersBoard(const std::string &positions) : _board(positions) {
		if (positions.at(64) == 'b')
			_whosTurn = MOVE_BLACK;
		if (positions.at(64) == 'r')
			_whosTurn = MOVE_RED;
	}
	// геттеры полей
	std::string getBoard() const {
		return _board;
	}
	char getCoordStatus(const int &pos) const {
		return _board.at(pos);
	}
	bool getWhosTurn() const {
		return _whosTurn;
	}
	void printBoard() const {
		for (unsigned int i = 0; i < _board.size(); ++i) {
			if (i != 0 && i % 8 == 0) {
				std::cout << std::endl;
			}
			std::cout << _board.at(i);
		}
		std::cout << "\n\n";
	}
};

// узел дерева
class TreeNode {
	CheckersBoard 				_cb; // доска
	TreeNode* 					_parent; // указатель на родительский узел
	std::vector<TreeNode*> 		_children; // массив всех возможных следующих ходов
	std::string 				_prevMove; // нотация предыдущего хода
	unsigned int 				_depth; // глубина узла в дереве
	double 						_value, _alpha, _beta; // значения очков, альфа, бета
	int 						_win; // обазначение победы одной из стороны

public:
	TreeNode() {}
	TreeNode(const CheckersBoard& start, std::string moveNotation) :
		_cb(start), _depth(0), _parent(nullptr), _prevMove(moveNotation),
		_value(0),
		_alpha(-std::numeric_limits<double>::infinity()),
		_beta(std::numeric_limits<double>::infinity()),
		_win(0)
	{
		std::string board = this->getCB().getBoard();
		board.pop_back();
		if (std::count(board.begin(), board.end(), 'b') == 0
			&& std::count(board.begin(), board.end(), 'B') == 0) {
			_win = (int)Win::winRed;
		}
		if (std::count(board.begin(), board.end(), 'r') == 0
			&& std::count(board.begin(), board.end(), 'R') == 0) {
			_win = (int)Win::winBlack;
		}

		if (this->getWin() == (int)Win::winRed) {
			_value = WIN_VALUE;
		}
		if (this->getWin() == (int)Win::winBlack) {
			_value = (-1) * WIN_VALUE;
		}
	}
	// геттеры полей
	CheckersBoard getCB() const {
		return _cb;
	}
	TreeNode* getParent() const {
		return _parent;
	}
	std::vector<TreeNode*> getChildren() const {
		return _children;
	}
	unsigned int getDepth() const {
		return _depth;
	}
	double getValue() const {
		return _value;
	}
	double getAlpha() const {
		return _alpha;
	}
	double getBeta() const {
		return _beta;
	}
	std::string getNotation() const {
		return _prevMove;
	}
	int getWin() const {
		return _win;
	}
	// сеттеры полей
	void setValue(double val) {
		_value = val;
	}
	void setAlpha(double a) {
		_alpha = a;
	}
	void setBeta(double b) {
		_beta = b;
	}
	void setParent(TreeNode *theParent) {
		_parent = theParent;
	}
	void addChild(TreeNode *child) {
		child->_depth = this->_depth + 1;
		child->setParent(this);
		_children.push_back(child);
	}
	void setWin(int win) {
		_win = win;
	}
};

// класс "шашки". тут описаны правила игры. тут строится дерево всех возможных ходов
class Checkers {
	std::queue<TreeNode*> 		_nodeQ; // очередь узлов для их обработки
	TreeNode* 					_currentNode; // указатель на текущий узел

	bool _isLegaltoKing(int finalPos) // проверка может ли обычна шашка стать королем
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
	void _createNoFightNode(int startPos, int finalPos) // создание дочернего узла при тихом ходе
	{
		std::string tempBoard = _currentNode->getCB().getBoard();

		if (_isLegaltoKing(finalPos) && tempBoard[startPos] == 'r') {
			tempBoard[startPos] = 'R';
		}
		if (_isLegaltoKing(finalPos) && tempBoard[startPos] == 'b') {
			tempBoard[startPos] = 'B';
		}
		std::swap(tempBoard[startPos], tempBoard[finalPos]);

		std::string notation = std::to_string(startPos) + "-" + std::to_string(finalPos);

		if (_currentNode->getCB().getWhosTurn() == MOVE_RED) {
			tempBoard.at(64) = 'b';
		}
		else {
			tempBoard.at(64) = 'r';
		}

		_currentNode->addChild(new TreeNode(CheckersBoard(tempBoard), notation));
		_nodeQ.push(_currentNode->getChildren().back());
	}

	void _moveNoFightKing(int pos) // условия тихого хода короля
	{
		// move king up left
		if (pos % 8 != 0) {
			for (int i = pos - 9; (i > 0); i -= 9) {
				if (_currentNode->getCB().getBoard().at(i) == '0') {
					_createNoFightNode(pos, i);
					if (i % 8 == 0 || i < 8)
						break;
				}
				else break;
			}
		}
		// move king up right
		if (pos % 8 != 7) {
			for (int i = pos - 7; (i > 0); i -= 7) {
				if (_currentNode->getCB().getBoard().at(i) == '0') {
					_createNoFightNode(pos, i);
					if (i % 8 == 7 || i < 8)
						break;
				}
				else break;
			}
		}
		// move king down right
		if (pos % 8 != 7) {
			for (int i = pos + 9; (i < 63); i += 9) {
				if (_currentNode->getCB().getBoard().at(i) == '0') {
					_createNoFightNode(pos, i);
					if (i % 8 == 7 || i > 55)
						break;
				}
				else break;
			}
		}
		// move king down left
		if (pos % 8 != 0) {
			for (int i = pos + 7; (i < 63); i += 7) {
				if (_currentNode->getCB().getBoard().at(i) == '0') {
					_createNoFightNode(pos, i);
					if (i % 8 == 0 || i > 55)
						break;
				}
				else break;
			}
		}
	}
	void _moveNoFightRed(int pos) // условия тихого хода красной шашки
	{
		// SIMPLE MOVE LEFT
		if (pos % 8 != 0) {
			if (_currentNode->getCB().getBoard().at(pos + 7) == '0') {
				_createNoFightNode(pos, pos + 7);
			}
		}
		// SIMPLE MOVE RIGHT
		if (pos % 8 != 7) {
			if (_currentNode->getCB().getBoard().at(pos + 9) == '0') {
				_createNoFightNode(pos, pos + 9);
			}
		}
	}
	void _moveNoFightBlack(int pos) // условия тихого хода черной шашки
	{
		if (pos % 8 != 7) {
			if (_currentNode->getCB().getBoard().at(pos - 7) == '0') {
				_createNoFightNode(pos, pos - 7);
			}
		}
		if (pos % 8 != 0) {
			if (_currentNode->getCB().getBoard().at(pos - 9) == '0') {
				_createNoFightNode(pos, pos - 9);
			}
		}
	}

	void _moveCombatKing(int pos, std::string tempBoard, std::string tempNotation) // условия и создание дочернего узла при бое короля
	{
		char friendPiece, friendKing, oppoPiece, oppoKing;
		if (_currentNode->getCB().getWhosTurn() == MOVE_RED) {
			friendPiece = 'r';
			friendKing = 'R';
			oppoPiece = 'b';
			oppoKing = 'B';
		}
		if (_currentNode->getCB().getWhosTurn() == MOVE_BLACK) {
			friendPiece = 'b';
			friendKing = 'B';
			oppoPiece = 'r';
			oppoKing = 'R';
		}

		bool combat = false;
		// UP LEFT
		if (pos % 8 > 1) {
			for (int i = pos - 9; i > 0; i -= 9) {
				if (i % 8 != 0 && i > 8) {
					if (tempBoard.at(i) == oppoPiece ||
						tempBoard.at(i) == oppoKing) {
						if (tempBoard.at(i - 9) == '0') {
							combat = true;

							std::string tempBoard1 = tempBoard;
							std::swap(tempBoard1[pos], tempBoard1[i - 9]);
							tempBoard1[i] = '0';

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
						if (tempBoard.at(i - 7) == '0') {
							combat = true;

							std::string tempBoard1 = tempBoard;
							std::swap(tempBoard1[pos], tempBoard1[i - 7]);
							tempBoard1[i] = '0';

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
						if (tempBoard.at(i + 9) == '0') {
							combat = true;

							std::string tempBoard1 = tempBoard;
							std::swap(tempBoard1[pos], tempBoard1[i + 9]);
							tempBoard1[i] = '0';

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
						if (tempBoard.at(i + 7) == '0') {
							combat = true;

							std::string tempBoard1 = tempBoard;
							std::swap(tempBoard1[pos], tempBoard1[i + 7]);
							tempBoard1[i] = '0';

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
			tempBoard.at(64) = 'r';
			_currentNode->addChild(new TreeNode(CheckersBoard(tempBoard), tempNotation));
			_nodeQ.push(_currentNode->getChildren().back());
			return;
		}
	}
	void _moveCombatRed(int pos, std::string tempBoard, std::string tempNotation) // условия и создание дочернего узла при бое красной шашки
	{
		// CHECK FORWARD MOVE LEFT
		bool combat = false;
		if (pos <= 46) {
			if (tempBoard[pos + 7] == 'b'
				&& tempBoard[pos + 7 + 7] == '0' && (pos % 8) > 1) {

				combat = true;

				std::string tempBoard1 = tempBoard;
				std::swap(tempBoard1[pos], tempBoard1[pos + 7 + 7]);
				tempBoard1[pos + 7] = '0';

				std::string notation = tempNotation + 'x'
					+ std::to_string(pos + 7 + 7);

				if (pos + 7 + 7 >= 56) {
					tempBoard1[pos + 7 + 7] = 'R';
					_moveCombatKing(pos + 7 + 7, tempBoard1, notation);
				}
				else {
					_moveCombatRed(pos + 7 + 7, tempBoard1, notation);
				}
			}
		}
		if (pos <= 44) {
			// CHECK FORWARD MOVE RIGHT
			if (tempBoard[pos + 9] == 'b'
				&& tempBoard[pos + 9 + 9] == '0' && (pos % 8) < 6) {

				combat = true;

				std::string tempBoard1 = tempBoard;
				std::swap(tempBoard1[pos], tempBoard1[pos + 9 + 9]);
				tempBoard1[pos + 9] = '0';

				std::string notation = tempNotation + 'x'
					+ std::to_string(pos + 9 + 9);

				if (pos + 9 + 9 >= 56) {
					tempBoard1[pos + 9 + 9] = 'R';
					_moveCombatKing(pos + 9 + 9, tempBoard1, notation);
				}
				else {
					_moveCombatRed(pos + 9 + 9, tempBoard1, notation);
				}
			}
		}
		if (pos >= 19) {
			// CHECK BACK MOVE LEFT
			if (tempBoard[pos - 9] == 'b'
				&& tempBoard[pos - 9 - 9] == '0' && (pos % 8) > 1
				&& (pos > 16)) {

				combat = true;

				std::string tempBoard1 = tempBoard;
				std::swap(tempBoard1[pos], tempBoard1[pos - 9 - 9]);
				tempBoard1[pos - 9] = '0';

				std::string notation = tempNotation + 'x'
					+ std::to_string(pos - 9 - 9);

				_moveCombatRed(pos - 9 - 9, tempBoard1, notation);
			}
		}
		if (pos >= 17) {
			// CHECK BACK MOVE RIGHT
			if (tempBoard[pos - 7] == 'b'
				&& tempBoard[pos - 7 - 7] == '0' && (pos % 8) < 6
				&& (pos > 16)) {

				combat = true;

				std::string tempBoard1 = tempBoard;
				std::swap(tempBoard1[pos], tempBoard1[pos - 7 - 7]);
				tempBoard1[pos - 7] = '0';

				std::string notation = tempNotation + 'x'
					+ std::to_string(pos - 7 - 7);

				_moveCombatRed(pos - 7 - 7, tempBoard1, notation);
			}
		}
		// END RECURSION
		if (tempBoard != _currentNode->getCB().getBoard() && combat == false) {
			tempBoard.at(64) = 'b';
			_currentNode->addChild(new TreeNode(CheckersBoard(tempBoard), tempNotation));
			_nodeQ.push(_currentNode->getChildren().back());
			return;
		}
	}
	void _moveCombatBlack(int pos, std::string tempBoard, std::string tempNotation) // условия и создание дочернего узла при бое черной шашки
	{
		bool combat = false;
		if (pos >= 19) {
			// CHECK FORWARD MOVE LEFT
			if (tempBoard[pos - 9] == 'r'
				&& tempBoard[pos - 9 - 9] == '0' && (pos % 8) > 1
				&& (pos > 16)) {

				combat = true;

				std::string tempBoard1 = tempBoard;
				std::swap(tempBoard1[pos], tempBoard1[pos - 9 - 9]);
				tempBoard1[pos - 9] = '0';

				std::string notation = tempNotation + 'x'
					+ std::to_string(pos - 9 - 9);

				if (pos - 9 - 9 <= 7) {
					tempBoard1[pos - 9 - 9] = 'B';
					_moveCombatKing(pos - 9 - 9, tempBoard1, notation);
				}
				else {
					_moveCombatRed(pos - 9 - 9, tempBoard1, notation);
				}
			}
		}
		if (pos >= 17) {
			// CHECK FORWARD MOVE RIGHT
			if (tempBoard[pos - 7] == 'r'
				&& tempBoard[pos - 7 - 7] == '0' && (pos % 8) < 6
				&& (pos > 16)) {

				combat = true;

				std::string tempBoard1 = tempBoard;
				std::swap(tempBoard1[pos], tempBoard1[pos - 7 - 7]);
				tempBoard1[pos - 7] = '0';

				std::string notation = tempNotation + 'x'
					+ std::to_string(pos - 7 - 7);

				if (pos - 7 - 7 <= 7) {
					tempBoard1[pos - 7 - 7] = 'B';
					_moveCombatKing(pos - 7 - 7, tempBoard1, notation);
				}
				else {
					_moveCombatRed(pos - 7 - 7, tempBoard1, notation);
				}
			}
		}
		if (pos <= 46) {
			// CHECK BACK MOVE LEFT
			if (tempBoard[pos + 7] == 'r'
				&& tempBoard[pos + 7 + 7] == '0' && (pos % 8) > 1) {

				combat = true;

				std::string tempBoard1 = tempBoard;
				std::swap(tempBoard1[pos], tempBoard1[pos + 7 + 7]);
				tempBoard1[pos + 7] = '0';

				std::string notation = tempNotation + 'x'
					+ std::to_string(pos + 7 + 7);

				_moveCombatBlack(pos + 7 + 7, tempBoard1, notation);
			}
		}
		if (pos <= 44) {
			// CHECK BACK MOVE RIGHT
			if (tempBoard[pos + 9] == 'r'
				&& tempBoard[pos + 9 + 9] == '0' && (pos % 8) < 6) {

				combat = true;

				std::string tempBoard1 = tempBoard;
				std::swap(tempBoard1[pos], tempBoard1[pos + 9 + 9]);
				tempBoard1[pos + 9] = '0';

				std::string notation = tempNotation + 'x'
					+ std::to_string(pos + 9 + 9);

				_moveCombatBlack(pos + 9 + 9, tempBoard1, notation);
			}
		}
		// END RECURSION
		if (tempBoard != _currentNode->getCB().getBoard() && combat == false) {
			tempBoard.at(64) = 'r';
			_currentNode->addChild(new TreeNode(CheckersBoard(tempBoard), tempNotation));
			_nodeQ.push(_currentNode->getChildren().back());
			return;
		}
	}

	void _makeMove() // попытка сделать ход
	{
		// для каждой клетки текущего узла 
		for (unsigned int i = 0; i < _currentNode->getCB().getBoard().size() - 1; ++i) {
			// если ход красных
			if ((_currentNode->getCB().getBoard().at(i) == 'r' || _currentNode->getCB().getBoard().at(i) == 'R')
				&& _currentNode->getCB().getWhosTurn() == MOVE_RED) {

				auto currChildSize = _currentNode->getChildren().size();
				// Проверка возможности вступить в бой если обычная шашка
				if (_currentNode->getCB().getBoard().at(i) == 'r') {
					_moveCombatRed(i, _currentNode->getCB().getBoard(), std::to_string(i));
				}
				// Проверка возможности вступить в бой если король
				if (_currentNode->getCB().getBoard().at(i) == 'R') {
					_moveCombatKing(i, _currentNode->getCB().getBoard(), std::to_string(i));
				}
				// если боя обычной шашки не было (очередь не увеличилась), то попытка тихого хода
				if (_currentNode->getChildren().size() == currChildSize &&
					(i < 56) && _currentNode->getCB().getBoard().at(i) == 'r')
				{
					_moveNoFightRed(i);
				}
				// если боя короля не было (очередь не увеличилась), то попытка тихого хода
				if (_currentNode->getChildren().size() == currChildSize &&
					_currentNode->getCB().getBoard().at(i) == 'R')
				{
					_moveNoFightKing(i);
				}
			}
			// если ход черных
			if ((_currentNode->getCB().getBoard().at(i) == 'b' || _currentNode->getCB().getBoard().at(i) == 'B') &&
				_currentNode->getCB().getWhosTurn() == MOVE_BLACK) {

				auto currChildSize = _currentNode->getChildren().size();
				// Проверка возможности вступить в бой если обычная шашка
				if (_currentNode->getCB().getBoard().at(i) == 'b') {
					_moveCombatBlack(i, _currentNode->getCB().getBoard(), std::to_string(i));
				}
				// Проверка возможности вступить в бой если король
				if (_currentNode->getCB().getBoard().at(i) == 'B') {
					_moveCombatKing(i, _currentNode->getCB().getBoard(), std::to_string(i));
				}
				// если боя обычной шашки не было (очередь не увеличилась), то попытка тихого хода
				if (_currentNode->getChildren().size() == currChildSize &&
					(i > 7) && _currentNode->getCB().getBoard().at(i) == 'b')
				{
					_moveNoFightBlack(i);
				}
				// если боя короля не было (очередь не увеличилась), то попытка тихого хода
				if (_currentNode->getChildren().size() == currChildSize &&
					_currentNode->getCB().getBoard().at(i) == 'B') {
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
	void _recursiveBuildTree(unsigned int currDepth, unsigned int depth) // рекурсивное построение дерева
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
			// на последнем уровне происходит
			// инициализация value для последнего слоя
			unsigned int staticQSize = _nodeQ.size();
			for (unsigned int i = 0; i < staticQSize; ++i) {
				_currentNode = _nodeQ.front();
				_currentNode->setValue(_evaluateValue(_currentNode));
				_nodeQ.pop();
			}
			return;
		}
	}

	double _evaluateValue(TreeNode*& node) // функция расчета очков
	{
		if (node->getWin() == (int)Win::winRed ||
			node->getWin() == (int)Win::winBlack)
			return node->getValue();

		std::string board = node->getCB().getBoard();
		double _val = (std::count(board.begin(), board.end(), 'r') + KING_COEF*(std::count(board.begin(), board.end(), 'R')))
			- (std::count(board.begin(), board.end(), 'b') + (KING_COEF)*(std::count(board.begin(), board.end(), 'B')));
		if (node->getCB().getWhosTurn() == MOVE_BLACK)
			++_val;
		if (node->getCB().getWhosTurn() == MOVE_RED)
			--_val;

		return _val;
	}

	// рекурсивные функции алгоритма "альфа-бета отсечение"
	double _maxValue(TreeNode*& node, double alpha, double beta) {
		if (node->getChildren().size() == 0) {
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
	double _minValue(TreeNode*& node, double alpha, double beta) {
		if (node->getChildren().size() == 0) {
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
public:
	Checkers() { _currentNode = nullptr; }
	Checkers(TreeNode* rootNode) {
		_currentNode = rootNode;
	}
	void buildTree(int depth) // начало построения дерева 
	{
		if (_currentNode->getParent() == nullptr) {
			if (_currentNode->getWin() == (int)Win::winNo) {
				_makeMove();
				_recursiveBuildTree(0, depth);
			}
		}
	}
	double evaluateBestMove(TreeNode*& root) // запуск функции расчета лучшего балла
	{
		if (root->getCB().getWhosTurn() == MOVE_RED)
			return _maxValue(root, root->getAlpha(), root->getBeta());
		if (root->getCB().getWhosTurn() == MOVE_BLACK) {
			return _minValue(root, root->getAlpha(), root->getBeta());
		}
	}
	std::string getBestScoreNotation(TreeNode*& root) // получение нотации лучшего хода 
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
		for (unsigned int i = 0; i < root->getChildren().size(); ++i) {
			if (root->getChildren().at(i)->getValue() == bestScore) {
				return root->getChildren().at(i)->getNotation();
			}
		}
		return "Error";
	}
};

// функция записи в файл
void writeNotationToFile(std::string notation, char* outputName) {
	std::ofstream answerFile;
	answerFile.open(outputName);
	answerFile << notation;
	answerFile.close();
}

// проверка входных данных на ошибки
int errorCheck(std::string init) {
	if (init.size() != 66) {
		return (int)InputError::badSize;
	}

	unsigned int depth = (int)init.at(65) - 48;
	if (depth < 0 || depth > 9) {
		return (int)InputError::badDepth;
	}
	init.pop_back();

	char move = init.at(64);
	if (move != 'r' && move != 'b') {
		return (int)InputError::badStartTurn;
	}
	init.pop_back();

	int zeroes = 0;
	for (unsigned int i = 0; i < init.size(); ++i) {
		if (init.at(i) != '0' &&
			init.at(i) != 'r' &&
			init.at(i) != 'R' &&
			init.at(i) != 'b' &&
			init.at(i) != 'B') {
			return (int)InputError::badCharOnBoard;
		}
		if (init.at(i) == '0') {
			++zeroes;
		}
	}

	if (zeroes == init.size()) {
		return (int)InputError::emptyBoard;
	}

	return (int)InputError::noError;
}

int main(int argc, char* argv[]) {
	std::string answer = "Error";

	// сохранение входных данных и запись номера ошибки в ответ
	std::string init = getStringStartPos(std::string(argv[1]));
	answer += std::to_string(errorCheck(init));

	// если ошибки нет
	if (answer.back() == '0') {
		// узнаем глубину построения дерева
		unsigned int depth = (int)init.at(65) - 48;
		init.pop_back();

		// создаётся доска
		CheckersBoard cb1(init);

		// создается указатель на корень
		TreeNode* root = new TreeNode(cb1, "noPrev");

		// создаётся "шашечная партия"...
		Checkers checkers(root); // ...с началом в корне
		checkers.buildTree(depth); // начало построения дерева с глубиной depth
		answer = checkers.getBestScoreNotation(root); // перезапись нотации в answer
	}
	writeNotationToFile(answer, argv[2]); // запись ответа в файл

	return 0;
}