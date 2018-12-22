#include "TreeNode.hpp"

TreeNode::TreeNode(const CheckersBoard & start, std::string moveNotation) :
	_cb(start), _depth(0), _parent(nullptr), _prevMove(moveNotation),
	_value(0),
	_alpha(-std::numeric_limits<double>::infinity()),
	_beta(std::numeric_limits<double>::infinity()),
	_win(0)
{
	std::vector<int> board = this->getCB().getBoard();
	board.pop_back();
	if (std::count(board.begin(), board.end(), PIECE_BLACK) == 0
		&& std::count(board.begin(), board.end(), PIECE_BLACK_KING) == 0) {
		_win = (int)Win::winRed;
	}
	if (std::count(board.begin(), board.end(), PIECE_RED) == 0
		&& std::count(board.begin(), board.end(), PIECE_RED_KING) == 0) {
		_win = (int)Win::winBlack;
	}

	if (this->getWin() == (int)Win::winRed) {
		_value = WIN_VALUE;
	}
	if (this->getWin() == (int)Win::winBlack) {
		_value = (-1) * WIN_VALUE;
	}
}

CheckersBoard TreeNode::getCB() const
{
	return _cb;
}

TreeNode * TreeNode::getParent() const
{
	return _parent;
}

std::vector<TreeNode*> TreeNode::getChildren() const
{
	return _children;
}

unsigned int TreeNode::getDepth() const
{
	return _depth;
}

double TreeNode::getValue() const
{
	return _value;
}

double TreeNode::getAlpha() const
{
	return _alpha;
}

double TreeNode::getBeta() const
{
	return _beta;
}

std::string TreeNode::getNotation() const
{
	return _prevMove;
}

int TreeNode::getWin() const
{
	return _win;
}

double TreeNode::getExtraScore() const
{
	return _extraValue;
}

void TreeNode::setValue(double val)
{
	_value = val;
}

void TreeNode::setAlpha(double a)
{
	_alpha = a;
}

void TreeNode::setBeta(double b)
{
	_beta = b;
}

void TreeNode::setParent(TreeNode * theParent)
{
	_parent = theParent;
}

void TreeNode::addChild(TreeNode * child)
{
	child->_depth = this->_depth + 1;
	child->setParent(this);
	_children.push_back(child);
}

void TreeNode::setWin(int win)
{
	_win = win;
}

void TreeNode::setExtraScore(double score)
{
	_extraValue = score;
}

