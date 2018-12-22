#pragma once
#include "CheckersBoard.hpp"


class TreeNode {
	CheckersBoard 				_cb; // �����
	TreeNode* 					_parent; // ��������� �� ������������ ����
	std::vector<TreeNode*> 		_children; // ������ ���� ��������� ��������� �����
	std::string 				_prevMove; // ������� ����������� ����
	unsigned int 				_depth; // ������� ���� � ������
	double 						_value, _alpha, _beta; // �������� �����, �����, ����
	double						_extraValue;
	int 						_win; // ����������� ������ ����� �� �������

public:
	TreeNode() {}
	TreeNode(const CheckersBoard& start, std::string moveNotation);
	
	// ������� �����
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
	
	// ������� �����
	void setValue(double val);
	void setAlpha(double a);
	void setBeta(double b);
	void setParent(TreeNode *theParent);
	void addChild(TreeNode *child);
	void setWin(int win);
	void setExtraScore(double score);
};