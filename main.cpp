// Made by inxane
// This is an interpreter for a Befunge-like language called "Spider"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
std::string spaces(int spaceAmount) {
	std::ostringstream spaces;
	for (int i = 0; i < spaceAmount; i++) {
		spaces << ' ';
	}
	return spaces.str();
}
int longestLine(const std::vector<std::string> &lines) {
	int max = 0;
	for (const std::string& line : lines) {
		if (line.size() > max)
			max = line.size();
	}
	return max;
}
std::vector<std::string> readFileToVector(const std::string& filename) {
	std::vector<std::string> fileContents;
	std::ifstream file(filename.c_str());
	if (!file) {
		std::cout << "ERROR: Couldn't open code file" << std::endl;
		exit(-1);
	}
	std::string line;
	while (std::getline(file, line)) {
		if (line.size() > 0 && line.rfind("`", 0) != 0)
			fileContents.push_back(line);
	}
	file.close();
	return fileContents;
}
std::string fixLineLength(std::string line, int maxLength) {
	return line + spaces(maxLength - line.size());
}
void fixLineLengths(std::vector<std::string>& fileContents, int longestLength) {
	for (int i = 0; i < fileContents.size(); i++)
		fileContents[i] = fixLineLength(fileContents[i], longestLength);
}
enum Direction {
	Up,
	Down,
	Left,
	Right
};
class Coordinates {
	public:
		int x;
		int y;
		Direction dir;
};
void safeMove(Coordinates& coords, Direction dir, int maxX, int maxY) {
	coords.dir = dir;
	switch (dir) {
		case Up:
			if (coords.y > 0)
				coords.y = coords.y - 1;
			else
				coords.y = maxY;
			break;
		case Down:
			if (coords.y == maxY)
				coords.y = 0;
			else
				coords.y = coords.y + 1;
			break;
		case Left:
			if (coords.x > 0)
				coords.x = coords.x - 1;
			else
				coords.x = maxX;
			break;
		case Right:
			if (coords.x == maxX)
				coords.x = 0;
			else
				coords.x = coords.x + 1;
		default:
			break;
	}
}
class Stack {
	private:
		std::vector<int> stack;
	public:
		int pop() {
			int result = 0;
			if (stack.size() > 0) {
				result = stack.back();
				stack.pop_back();
			}
			return result;
		}
		void push(int value) {
			stack.push_back(value);
		}
		int length() {
			return stack.size();
		}
		void reverse() {
			std::reverse(stack.begin(), stack.end());
		}
		std::string toString() {
			std::ostringstream output;
			output << "[";
			for (int i = 0; i < stack.size(); i++)
				output << stack.at(i) << (i != stack.size() - 1 ? ", " : "");
			output << "]";
			return output.str();
		}
		void dup() {
			stack.push_back(stack.back());
		}
		void clear() {
			stack.clear();
		}
};
void clearStringStream(std::ostringstream& stream) {
	stream.str("");
	stream.clear();
}
int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cout << "ERROR: File name not specified!" << std::endl;
		return 0;
	}
	auto fileContents = readFileToVector(argv[1]);
	fixLineLengths(fileContents, longestLine(fileContents));
	// Code information
	int maxX = fileContents[0].size() - 1;
	int maxY = fileContents.size() - 1;
	// Setup instruction pointer
	Coordinates coords;
	coords.x = 0;
	coords.y = 0;
	coords.dir = Right;
	// Language features
	Stack stack;
	bool stop = false;
	bool debug = false;
	bool stringMode = false;
	bool numberMode = false;
	int chance;
	if (debug) {
		for (std::string line : fileContents) {
			std::cout << line << line.length() << std::endl;
		}
	}
	
	std::ostringstream numberModeStream;
	// Variables to prevent cross-initialization
	std::string input;
	const std::vector<Direction> dirs = {Up, Down, Left, Right};
	// Code loop
	srand(time(NULL));
	while (!stop) {
		char cur = fileContents[coords.y][coords.x];
		if (debug) {
			std::cout << "[DEBUG] coords.x = "
					  << coords.x 
					  << ", coords.y = " 
					  << coords.y 
					  << ", coords.dir = " 
					  << coords.dir 
					  << ", cur = " 
					  << cur 
					  << std::endl;			
		}
		if (stringMode && cur != '"') {
			stack.push((int)cur);
			safeMove(coords, coords.dir, maxX, maxY);
			continue;
		}
		if (numberMode && cur != ')') {
			if ((int)cur >= 48 && (int)cur <= 57)
				numberModeStream << cur;
			safeMove(coords, coords.dir, maxX, maxY);
			continue;
		}
		switch (cur) {
			case '>':
				safeMove(coords, Right, maxX, maxY);
				continue;
			case '<':
				safeMove(coords, Left, maxX, maxY);
				continue;
			case '^':
				safeMove(coords, Up, maxX, maxY);
				continue;
			case 'v':
				safeMove(coords, Down, maxX, maxY);
				continue;
			case '0' ... '9':
				stack.push(((int)cur) - 48);
				break;
			case 'o':
				std::cout << stack.pop();
				break;
			case 'p':
				std::cout << (char)stack.pop();
				break;
			case '@':
				stop = true;
				break;
			case '+':
				stack.push(stack.pop() + stack.pop());
				break;
			case '-':
				stack.push(stack.pop() - stack.pop());
				break;
			case '*':
				stack.push(stack.pop() * stack.pop());
				break;
			case '=':
				if (stack.pop() == stack.pop())
					safeMove(coords, Up, maxX, maxY);
				else
					safeMove(coords, Down, maxX, maxY);
				continue;
			case '"':
				stringMode = !stringMode;
				break;
			case 'g':
				input = "";
				std::getline(std::cin, input);
				stack.push((int)input[0]);
				break;
			case 'G':
				input = "";
				std::getline(std::cin, input);
				for (char c : input)
					stack.push((int)c);
				break;
			case 'i':
				input = "";
				std::getline(std::cin, input);
				stack.push(std::stoi(input));
				break;
			case 'O':
				while (stack.length() > 0)
					std::cout << (char)stack.pop();
				break;
			case 'd':
				stack.dup();
				break;
			case 'D':
				stack.pop();
				break;
			case 'r':
				stack.reverse();
				break;
			case 'l':
				stack.push(stack.length());
				break;
			case 's':
				std::cout << stack.toString();
				break;
			case '(':
				numberMode = !numberMode;
				clearStringStream(numberModeStream);
				break;
			case ')':
				numberMode = false;
				stack.push(std::stoi(numberModeStream.str()));
				clearStringStream(numberModeStream);
				break;
			case '#':
				safeMove(coords, dirs.at(rand() % 4), maxX, maxY);
				continue;
			case '_':
				safeMove(coords, coords.dir, maxX, maxY);
				safeMove(coords, coords.dir, maxX, maxY);
				continue;
			case 'N':
				std::cout << std::endl;
				break;
			case 'C':
				stack.clear();
				break;
			case '%':
				chance = stack.pop();
				if ((rand() % chance) + 1 == chance)
					safeMove(coords, Up, maxX, maxY);
				else
					safeMove(coords, Down, maxX, maxY);
				continue;
			default:
				break;
		}
		safeMove(coords, coords.dir, maxX, maxY);
	}
}
