#pragma once
#include "CheckersBoard.hpp"


class TreeNode {
	CheckersBoard 				_cb; // доска
	TreeNode* 					_parent; // указатель на родительский узел
	std::vector<TreeNode*> 		_children; // массив всех возможных следующих ходов
	std::string 				_prevMove; // нотация предыдущего хода
	unsigned int 				_depth; // глубина узла в дереве
	double 						_value, _alpha, _beta; // значения очков, альфа, бета
	double						_extraValue;
	int 						_win; // обазначение победы одной из стороны

public:
	TreeNode() {}
	TreeNode(const CheckersBoard& start, std::string moveNotation);
	
	// геттеры полей
	CheckersBoard getCB() const;
	TreeNode* getParent() const;
	std::vector<TreeNode*> getChildren() const;
	unsigned int getDepth() const;
	double getValue() const;
	double getAlpha() const;
	double getBeta() const;
	std::string getNotation() const;
	int getWin() const;
	double getExtraScore() const;
	
	// сеттеры полей
	void setValue(double val);
	void setAlpha(double a);
	void setBeta(double b);
	void setParent(TreeNode *theParent);
	void addChild(TreeNode *child);
	void setWin(int win);
	void setExtraScore(double score);
};