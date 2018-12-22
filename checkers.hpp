#pragma once
#include "TreeNode.hpp"

class Checkers {
	std::queue<TreeNode*> 		_nodeQ; // очередь узлов для их обработки
	TreeNode* 					_currentNode; // указатель на текущий узел

	bool _isLegaltoKing(int finalPos);
	void _createNoFightNode(int startPos, int finalPos);

	void _moveNoFightKing(int pos);
	void _moveNoFightRed(int pos);
	void _moveNoFightBlack(int pos);

	void _moveCombatKing(int pos, std::vector<int> tempBoard, std::string tempNotation);
	void _moveCombatRed(int pos, std::vector<int> tempBoard, std::string tempNotation);
	void _moveCombatBlack(int pos, std::vector<int> tempBoard, std::string tempNotation);

	void _makeMove();
	void _recursiveBuildTree(unsigned int currDepth, unsigned int depth);

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