#include <limits>
#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
static int counter = 0;

class TreeNode {
public:
	TreeNode* _parent;
	std::vector<TreeNode*> _children;
	double _value, _alpha, _beta;
	bool _max;
	unsigned int _depth;
	
	TreeNode() : _parent(nullptr),
		_value(0),
		_alpha(-std::numeric_limits<double>::infinity()),
		_beta(std::numeric_limits<double>::infinity()),
		_max(true) {}
	
	void setParent(TreeNode *theParent) {
		_parent = theParent;
	}
	void addChild(TreeNode *child) {
		child->_max = !_max;
		child->_depth = this->_depth + 1;
		child->setParent(this);
		_children.push_back(child);
	}
};

class Tree {
public:
	TreeNode *currentNode;
	std::queue<TreeNode*> _nodeQ;

	Tree(){}
	Tree(TreeNode* root) {
		currentNode = root;
	}
	double _maxValue(TreeNode*& node, double alpha, double beta) {
		counter++;
		node->_alpha = alpha;
		node->_beta = beta;
		if (node->_children.size() == 0) {
			return node->_value;
		}
		node->_value = -std::numeric_limits<double>::infinity();
		for (unsigned int i = 0; i < node->_children.size(); ++i) {
			double tempValue = _minValue(
				node->_children.at(i), node->_alpha, node->_beta);
			if (tempValue > node->_value)
				node->_value = tempValue;
			if (tempValue >= node->_beta)
				return node->_value;
			if (tempValue > node->_alpha)
				node->_alpha = tempValue;
		}
		return node->_value;
	}
	double _minValue(TreeNode*& node, double alpha, double beta) {
		counter++;
		node->_alpha = alpha;
		node->_beta = beta;
		if (node->_children.size() == 0) {
			return node->_value;
		}
		node->_value = std::numeric_limits<double>::infinity();
		for (unsigned int i = 0; i < node->_children.size(); ++i) {
			double tempValue = _maxValue(
				node->_children.at(i), node->_alpha, node->_beta);
			if (tempValue < node->_value)
				node->_value = tempValue;
			if (tempValue <= node->_alpha)
				return node->_value;
			if (tempValue < node->_beta)
				node->_beta = tempValue;
		}
		return node->_value;
	}
	void buildTree(unsigned int currDepth, unsigned int depth) {
		if (currDepth == 0) {
			for (unsigned int i = 0; i < 2; ++i) {
				currentNode->addChild(new TreeNode());
				_nodeQ.push(currentNode->_children.back());
			}
		}
		currDepth++;
		if (currDepth < depth) {
			unsigned int staticQSize = _nodeQ.size();
			for (unsigned int i = 0; i < staticQSize; ++i) {
				currentNode = _nodeQ.front();
				for (unsigned int i = 0; i < 2; ++i) {
					currentNode->addChild(new TreeNode());
					_nodeQ.push(currentNode->_children.back());
				}
				_nodeQ.pop();
			}
			if (currDepth + 1 == depth) {
				unsigned int staticQSize = _nodeQ.size();
				for (unsigned int i = 0; i < staticQSize; ++i) {
					currentNode = _nodeQ.front();
					if (i == 0)
						currentNode->_value = 4;
					if (i == 1)
						currentNode->_value = 8;
					if (i == 2)
						currentNode->_value = 9;
					if (i == 3)
						currentNode->_value = 3;
					if (i == 4)
						currentNode->_value = 2;
					if (i == 5)
						currentNode->_value = -2;
					if (i == 6)
						currentNode->_value = 9;
					if (i == 7)
						currentNode->_value = -1;
					_nodeQ.pop();
				}
			}
			buildTree(currDepth, depth);
		}
	}
	double evaluateBestMove(TreeNode*& root) {
		if (root->_max == 1)
			return _maxValue(root, root->_alpha, root->_beta);
		if (root->_max == 0) {
			return _minValue(root, root->_alpha, root->_beta);
		}
	}
};

void writeToFile(double bestMove, char* outputName) {
	std::ofstream answerFile;
	answerFile.open(outputName);
	answerFile << "Optimal Move Score: " << bestMove << std::endl;
	answerFile << "Number of calls: " << counter << std::endl;
	answerFile.close();
}

int main(int argc, char* argv[]) {
	TreeNode *root = new TreeNode();
	Tree tr(root);
	tr.buildTree(0, 3);
	double hm = tr.evaluateBestMove(root);
	
	writeToFile(hm, argv[1]);

	return 0;
}