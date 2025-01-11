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
#include <queue>
#include <cmath>
#include <utility> // For std::swap

const char* INPUT_FILE_PATH = "..\\..\\DAY15\\input\\input.txt";


typedef std::vector<std::vector<char>> Grid;

struct GameState {
	Grid grid;
	std::vector<char> instructions;
};

// Function to split a string by a delimiter
std::vector<std::string> splitString(const std::string& str, const std::string& delimiter) {
	std::vector<std::string> result;
	size_t start = 0;
	size_t end = str.find(delimiter);

	while (end != std::string::npos) {
		result.emplace_back(str.substr(start, end - start));
		start = end + delimiter.length();
		end = str.find(delimiter, start);
	}

	// Add the last part
	result.emplace_back(str.substr(start));

	return result;
}


GameState read_input(const char* file_path) {
	std::ifstream file(file_path);
	if (!file) {
		char buffer[256]; // Buffer to store the error message
		strerror_s(buffer, sizeof(buffer), errno);
		std::cerr << "Unable to open file: " << buffer << '\n';
		throw std::runtime_error("Failed to open file");
	}

	GameState state = {};

	Grid grid = {};
	std::vector<char> instructions = {};

	std::ostringstream buffer;
	buffer << file.rdbuf();
	std::string fileContent = buffer.str();

	std::vector<std::string> parts = splitString(fileContent, "\n\n");

	std::vector<char> row = {};
	for (const auto& ch : parts[0]) {
		if (ch != '\n') {
			row.emplace_back(ch);
		}
		else {
			grid.emplace_back(row);
			row.clear();
		}
	}
	grid.emplace_back(row);

	for (const auto& ch : parts[1]) {
		if (ch != '\n') {
			instructions.emplace_back(ch);
		}
	}

	state.grid = grid;
	state.instructions = instructions;

	return state;


}


void shift_block(int start, int end, Grid& grid, char dir, int axis) {
	switch (dir) {
		case '^':
			for (int i = end; i < start; ++i) {
				std::swap(grid[i][axis] , grid[i+1][axis]);
			}
			break;
		case '<':
			for (int i = end; i < start; ++i) {
				std::swap(grid[axis][i], grid[axis][i+1]);
			}
			break;
		case 'v':
			for (int i = end; i > start; --i) {
				std::swap(grid[i][axis], grid[i - 1][axis]);
			}
			break;
		case '>':
			for (int i = end; i > start; --i) {
				std::swap(grid[axis][i], grid[axis][i - 1]);
			}
			break;
	}
}



std::tuple<int, int> find_guard(Grid& grid) {
	std::tuple<int, int> guard_pos = {};
	for (int i = 0; i < grid.size(); ++i) {
		for (int j = 0; j < grid[i].size(); ++j) {
			if (grid[i][j] == '@') {
				guard_pos = { i, j };
			}
		}

	}
	return guard_pos;
}


int part1(GameState input) {

	
	int rows = (int)input.grid.size();
	int cols = (int)input.grid[0].size();

	std::tuple<int, int> guard_pos = find_guard(input.grid);

	for (const auto& inst : input.instructions) {
		int guard_x_pos = std::get<0>(guard_pos);
		int guard_y_pos = std::get<1>(guard_pos);
		bool can_move = false;
		switch (inst) {
			case '^':

				for (int i = guard_x_pos; i >= 0; --i) {
					if (input.grid[i][guard_y_pos] == '#') break;
					if (input.grid[i][guard_y_pos] == '.') {
						shift_block(guard_x_pos, i, input.grid, inst, guard_y_pos);
						can_move = true;
						break;
					}
				}
				if (can_move) {
					guard_pos = { guard_x_pos - 1, guard_y_pos };
					can_move = false;
				}
				
				break;

			case '<':
				for (int i = guard_y_pos; i >= 0; --i) {
					if (input.grid[guard_x_pos][i] == '#') break;
					if (input.grid[guard_x_pos][i] == '.') {
						shift_block(guard_y_pos, i, input.grid, inst, guard_x_pos);
						can_move = true;
						break;
					}
				}
				if (can_move) {
					guard_pos = { guard_x_pos, guard_y_pos - 1 };
					can_move = false;
				}
				
				break;
			case 'v':
				for (int i = guard_x_pos; i < rows; ++i) {
					if (input.grid[i][guard_y_pos] == '#') break;
					if (input.grid[i][guard_y_pos] == '.') {
						shift_block(guard_x_pos, i, input.grid, inst, guard_y_pos);
						can_move = true;
						break;
					}
				}
				if (can_move) {
					guard_pos = { guard_x_pos + 1, guard_y_pos };
					can_move = false;
				}

				break;
			case '>':
				for (int i = guard_y_pos; i < cols; ++i) {
					if (input.grid[guard_x_pos][i] == '#') break;
					if (input.grid[guard_x_pos][i] == '.') {
						shift_block(guard_y_pos, i, input.grid, inst, guard_x_pos);
						can_move = true;
						break;
					}
				}
				if (can_move) {
					guard_pos = { guard_x_pos, guard_y_pos + 1 };
					can_move = false;
				}

				break;
		}

	}
	int total = 0;
	for (int i = 0; i < input.grid.size(); ++i) {
		for (int j = 0; j < input.grid[i].size(); ++j) {
			if (input.grid[i][j] == 'O') {
				total += i * 100 + j;
			}
		}
	}

	return total;

}




int main() {

	GameState input = read_input(INPUT_FILE_PATH);

	int part1_answer = part1(input);

	std::cout << "PART 1: " << part1_answer << "\n";

	return 0;
}



