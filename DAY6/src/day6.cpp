#include <iostream>
#include <string>
#include <fstream>
#include <cstring> // For std::strerror
#include <cerrno>  // For errno
#include <vector>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <unordered_set>

const char* INPUT_FILE_PATH = "..\\..\\DAY6\\input\\input.txt";

typedef std::vector<std::vector<char>> Grid;

enum Direction {
	Up,
	Down,
	Right,
	Left
};

struct Position {
	int x, y;
	Direction dir;
	// Define equality operator
	bool operator==(const Position& other) const {
		return x == other.x && y == other.y;
	}
};


namespace std {
	template <>
	struct hash<Position> {
		size_t operator()(const Position& pos) const {
			// Hash individual members and combine them
			size_t h1 = std::hash<int>()(pos.x);
			size_t h2 = std::hash<int>()(pos.y);
			return h1 ^ (h2 << 1); // Combine the hashes
		}
	};
}


Grid read_input(const char* file_path) {
	Grid input;
	std::ifstream file(file_path);
	if (!file) {
		char buffer[256]; // Buffer to store the error message
		strerror_s(buffer, sizeof(buffer), errno);
		std::cerr << "Unable to open file: " << buffer << '\n';
		throw std::runtime_error("Failed to open file");
	}

	char ch;
	std::vector<char> row;
	while (file.get(ch)) {

		if (ch != '\n') {
			row.emplace_back(ch);
		}
		else {
			input.emplace_back(row);
			row.clear();
		}


	}
	input.emplace_back(row);

	file.close();

	return input;
}

Position find_guard(Grid input) {
	Position pos = { -1, -1 , Direction::Up};

	for (int i = 0; i < input.size(); ++i) {
		for (int j = 0; j < input[i].size(); ++j) {
			if (input[i][j] == '^') {
				pos = { i, j ,Direction::Up };
			}
		}
	}

	return pos;
}

size_t part1(Grid input) {
	//int result = 0;
	Position guard = find_guard(input);
	std::unordered_set<Position> hashSet;
	int y_pos = guard.x;
	int x_pos = guard.y;
	Direction dir = guard.dir;
	if (y_pos == -1 && x_pos == -1) {
		return 0;
	}

	while (y_pos < input.size() && x_pos < input[0].size() && y_pos >= 0 && x_pos >= 0) {
		if (input[y_pos][x_pos] != '#') {
			hashSet.insert(Position {y_pos, x_pos, dir});
		}
		else {
			if (dir == Direction::Up) {
				y_pos++;
				dir = Direction::Right;
			}
			else if (dir == Direction::Down) {
				y_pos--;
				dir = Direction::Left;
			}
			else if (dir == Direction::Left) {
				x_pos++;
				dir = Direction::Up;
			}
			else if (dir == Direction::Right) {
				x_pos--;
				dir = Direction::Down;
			}
		}

		if (dir == Direction::Up) {
			y_pos--;
		}
		else if (dir == Direction::Down) {
			y_pos++;
		}
		else if (dir == Direction::Left) {
			x_pos--;
		}
		else if (dir == Direction::Right) {
			x_pos++;
		}

		

	}

	return hashSet.size();
}

int main() {
	Grid input = read_input(INPUT_FILE_PATH);

	size_t part1_answer = part1(input);

	std::cout << "PART 1: " << part1_answer << std::endl;
	
	return 0;
}