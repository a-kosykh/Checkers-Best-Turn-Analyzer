#include "checkers.hpp"

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

std::vector<int> posVec(std::string positions) {
	std::vector<int> pos;
	for (unsigned int i = 0; i < positions.size() - 1; ++i) {
		if (positions.at(i) == '0')
			pos.push_back(PIECE_NO);
		if (positions.at(i) == 'r')
			pos.push_back(PIECE_RED);
		if (positions.at(i) == 'b') 
			pos.push_back(PIECE_BLACK);
		if (positions.at(i) == 'R')
			pos.push_back(PIECE_RED_KING);
		if (positions.at(i) == 'B')
			pos.push_back(PIECE_BLACK_KING);
	}

	return pos;
}

int main(int argc, char* argv[]) {
	std::string answer = "Error";

	// сохранение входных данных и запись номера ошибки в ответ
	std::string init = getStringStartPos(std::string(argv[1]));
	answer += std::to_string(errorCheck(init));

	std::string notation;
	// если ошибки нет
	if (answer.back() == '0') {
		
		
		// узнаем глубину построения дерева
		unsigned int depth = (int)init.at(65) - 48;
		init.pop_back();

		bool move = false;
		if (init.at(64) == 'b')
			move = true;

		// создаётся доска
		CheckersBoard cb1(posVec(init), move);

		// создается указатель на корень
		TreeNode* root = new TreeNode(cb1, "noPrev");

		// создаётся "шашечная партия"...
		Checkers checkers(root); // ...с началом в корне
		checkers.buildTree(depth); // начало построения дерева с глубиной depth
		answer = checkers.getBestScoreNotation(root); // перезапись нотации в answer

		// для вывода всех возможный ходов корня
		for (unsigned int i = 0; i < root->getChildren().size(); ++i) {
			notation = notation + root->getChildren().at(i)->getNotation() + " ";
		}
	}
	writeNotationToFile(answer , argv[2]); // запись ответа в файл

	return 0;
}