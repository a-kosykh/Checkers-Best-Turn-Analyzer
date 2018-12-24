#pragma once
#include "TreeNode.hpp"

class Checkers {
	std::queue<TreeNode*> 		_nodeQ; // очередь узлов для их обработки
	TreeNode* 					_currentNode; // указатель на текущий узел

	// возвращает id фигуры
	int _pieceOpponent(bool king, TreeNode* node);
	int _pieceFriend(bool king, TreeNode* node);
	
	// условия хода
	bool _isLegaltoKing(int finalPos);
	bool _isLegaltoCombatUpLeft(std::vector<int> tempBoard, int pos);
	bool _isLegaltoCombatUpRight(std::vector<int> tempBoard, int pos);
	bool _isLegaltoCombatDownRight(std::vector<int> tempBoard, int pos);
	bool _isLegaltoCombatDownLeft(std::vector<int> tempBoard, int pos);
	
	// вспомогательные функции
	std::vector<int> _swapCombatPieces(std::vector<int> tempBoard, int startPos, int nullPos,
		int finalPos, bool toKing);
	std::string _createNotation(std::string notation, int finalPos, bool isCombat);
	
	// создание узлов
	void _createNoFightNode(int startPos, int finalPos);

	void _moveNoFightKing(int pos);
	void _moveNoFightRed(int pos);
	void _moveNoFightBlack(int pos);

	void _moveCombatKing(int pos, std::vector<int> tempBoard, std::string tempNotation);
	void _moveCombatRed(int pos, std::vector<int> tempBoard, std::string tempNotation);
	void _moveCombatBlack(int pos, std::vector<int> tempBoard, std::string tempNotation);

	// запуск создания узлов (проход по доске)
	void _makeMove();
	
	// построение дерева
	void _recursiveBuildTree(unsigned int currDepth, unsigned int depth);

	// вычисление оценки
	double _evaluateValue(TreeNode*& node);

	// рекурсивные функции алгоритма "альфа-бета отсечение"
	double _maxValue(TreeNode*& node, double alpha, double beta);
	double _minValue(TreeNode*& node, double alpha, double beta);

public:
	Checkers() { _currentNode = nullptr; }
	Checkers(TreeNode* rootNode);
	void buildTree(int depth);
	double evaluateBestMove(TreeNode*& root);
	void evaluateExtraScore(TreeNode*& node);
	std::string getBestScoreNotation(TreeNode*& root);

};