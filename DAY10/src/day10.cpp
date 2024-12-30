#include <iostream>
#include <string>
#include <fstream>
#include <cstring> // For std::strerror
#include <cerrno>  // For errno
#include <vector>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <queue>


const char* INPUT_FILE_PATH = "..\\..\\DAY10\\input\\input.txt";
//const char* INPUT_FILE_PATH = "..\\..\\DAY10\\input\\sample.txt";



typedef std::vector<std::vector<char>> Grid;

struct TupleHash {
	std::size_t operator()(const std::tuple<int, int>& t) const {
		std::size_t h1 = std::hash<int>{}(std::get<0>(t));
		std::size_t h2 = std::hash<int>{}(std::get<1>(t));

		// Combine the two hash values
		return h1 ^ (h2 << 1); // XOR with a bit shift for better distribution
	}
};


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

int search_maze1(const Grid& input, int pos_x, int pos_y) {

	std::vector<std::tuple<int, int, int>> stack = {};
	std::unordered_set<std::tuple<int, int>, TupleHash> visited = {};

	std::vector<std::tuple<int, int>> dirs = {
				{-1,0},
				{1,0},
				{0,-1},
				{0,1}
	};

	int found = 0;
	std::tuple<int, int, int> cur_state = { pos_x, pos_y , 0 };
	stack.emplace_back(cur_state);

	while (!stack.empty()) {
		std::tuple<int, int, int> state = stack[stack.size() - 1];
		stack.pop_back();

		if (visited.count({ std::get<0>(state), std::get<1>(state) }) == 0) {

			visited.insert({ std::get<0>(state), std::get<1>(state) });

			if (std::get<2>(state) == 9) {
				found++;
				continue;
			}

			for (const auto& dir : dirs) {
				int new_row = std::get<0>(state) + std::get<0>(dir);
				int new_col = std::get<1>(state) + std::get<1>(dir);

				if (new_row < 0 || new_col < 0 || new_row >= input.size() || new_col >= input[0].size()) {
					continue;
				}

				if ((input[new_row][new_col] - '0') - (input[std::get<0>(state)][std::get<1>(state)] - '0') == 1) {
					std::tuple<int, int, int> new_state = { new_row, new_col, std::get<2>(state) + 1 };
					stack.emplace_back(new_state);
				}
			}
		}
		
	}

	return found;

}

int search_maze2(const Grid& input, int pos_x, int pos_y) {

	std::vector<std::tuple<int, int, int>> stack = {};

	std::vector<std::tuple<int, int>> dirs = {
				{-1,0},
				{1,0},
				{0,-1},
				{0,1}
	};

	int found = 0;
	std::tuple<int, int, int> cur_state = { pos_x, pos_y , 0 };
	stack.emplace_back(cur_state);

	while (!stack.empty()) {
		std::tuple<int, int, int> state = stack[stack.size() - 1];
		stack.pop_back();


		if (std::get<2>(state) == 9) {
			found++;
			continue;
		}

		for (const auto& dir : dirs) {
			int new_row = std::get<0>(state) + std::get<0>(dir);
			int new_col = std::get<1>(state) + std::get<1>(dir);

			if (new_row < 0 || new_col < 0 || new_row >= input.size() || new_col >= input[0].size()) {
				continue;
			}

			if ((input[new_row][new_col] - '0') - (input[std::get<0>(state)][std::get<1>(state)] - '0') == 1) {
				std::tuple<int, int, int> new_state = { new_row, new_col, std::get<2>(state) + 1 };
				stack.emplace_back(new_state);
			}
		}

	}

	return found;

}

int part1(Grid& input) {

	int total = 0;
	for (int i = 0; i < input.size(); ++i) {
		for (int j = 0; j < input[i].size(); ++j) {
			if (input[i][j] == '0') {
				int num_paths = search_maze1(input, i, j);
				total += num_paths;
			}
			
		}
	}

	return total;

}

int part2(Grid& input) {

	int total = 0;
	for (int i = 0; i < input.size(); ++i) {
		for (int j = 0; j < input[i].size(); ++j) {
			if (input[i][j] == '0') {
				int num_paths = search_maze2(input, i, j);
				total += num_paths;
			}

		}
	}

	return total;

}


int main() {
	Grid input = read_input(INPUT_FILE_PATH);

	int part1_answer = part1(input);

	std::cout << "PART 1: " << part1_answer << "\n";

	int part2_answer = part2(input);

	std::cout << "PART 2: " << part2_answer << "\n";

	return 0;
}