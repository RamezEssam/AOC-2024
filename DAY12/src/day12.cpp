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
#include <queue>



const char* INPUT_FILE_PATH = "..\\..\\DAY12\\input\\input.txt";


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

int calc_perim(const std::unordered_set<std::tuple<int, int>, TupleHash>& region, const Grid& input) {
	int perim = 0;

	int rows = (int)input.size();
	int cols = (int)input[0].size();

	std::vector<std::tuple<int, int>> dirs = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

	for (const auto& cell : region) {
		int sides = 0;
		int i = std::get<0>(cell);
		int j = std::get<1>(cell);

		for (const auto& dir : dirs) {

			int new_row = i + std::get<0>(dir);
			int new_col = j + std::get<1>(dir);

			if (new_row >= 0 && new_row < rows && new_col >= 0 && new_col < cols  && input[new_row][new_col] != input[i][j]) {
				sides += 1;
			}
			if (new_row < 0 || new_row >= rows || new_col < 0 || new_col >= cols) {
				sides += 1;
			}
		}
		perim += sides;
	}

	return perim;
}

int calc_perim2(const std::unordered_set<std::tuple<int, int>, TupleHash>& region, const Grid& input) {
	int perim = 0;

	int rows = (int)input.size();
	int cols = (int)input[0].size();
	
	int concave_corners = 0;
	int convex_corners = 0;

	for (const auto& cell : region) {

		int i = std::get<0>(cell);
		int j = std::get<1>(cell);

		// Convex Corners detection
		// 
		// top left corner
		if (((j-1 >= 0 && input[i][j-1] != input[i][j]) || (j-1 < 0)) && ((i-1 >= 0 && input[i-1][j] != input[i][j]) || (i-1 < 0))) {
			convex_corners++;
		}

		// top right corner
		if (((j + 1 < cols && input[i][j + 1] != input[i][j]) || (j + 1 >= cols)) && ((i - 1 >= 0 && input[i - 1][j] != input[i][j]) || (i - 1 < 0))) {
			convex_corners++;
		}

		// bottom left corner
		if (((j - 1 >= 0 && input[i][j - 1] != input[i][j]) || (j - 1 < 0)) && ((i + 1 < rows && input[i + 1][j] != input[i][j]) || (i + 1 >= rows))) {
			convex_corners++;
		}

		// bottom right corner
		if (((j + 1 < cols && input[i][j + 1] != input[i][j]) || (j + 1 >= cols)) && ((i + 1 < rows && input[i + 1][j] != input[i][j]) || (i + 1 >= rows))) {
			convex_corners++;
		}

		
		// Concave Corners detection
		//
		// top left corner
		if ((j - 1 >= 0 && input[i][j - 1] == input[i][j]) && (i - 1 >= 0 && input[i - 1][j] == input[i][j]) && (i-1 >= 0 && j-1 >= 0 && input[i-1][j-1] != input[i][j])) {
			concave_corners++;
		}

		// top right corner
		if ((j + 1 < cols && input[i][j + 1] == input[i][j]) && (i - 1 >= 0 && input[i - 1][j] == input[i][j]) && (i-1 >= 0 && j+1 < cols && input[i-1][j+1] != input[i][j])) {
			concave_corners++;
		}

		// bottom left corner
		if ((j - 1 >= 0 && input[i][j - 1] == input[i][j]) && (i + 1 < rows && input[i + 1][j] == input[i][j]) && (i+1 < rows && j-1 >= 0 && input[i+1][j-1] != input[i][j])) {
			concave_corners++;
		}

		// bottom right corner
		if ((j + 1 < cols && input[i][j + 1] == input[i][j]) && (i + 1 < rows && input[i + 1][j] == input[i][j]) && (i+1 < rows && j+1 < cols && input[i+1][j+1] != input[i][j])) {
			concave_corners++;
		}
		
		
	}

	perim = convex_corners + concave_corners;

	return perim;
}

int calcualte_region_price(const Grid& input, int pos_x, int pos_y, std::unordered_set<std::tuple<int, int>, TupleHash>& global_visited) {
	int rows = (int)input.size();
	int cols = (int)input[0].size();

	std::vector<std::tuple<int, int>> dirs = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

	std::unordered_set<std::tuple<int, int>, TupleHash> local_visited = {};

	std::queue<std::tuple<int, int>> q = {};

	q.push({ pos_x, pos_y });

	local_visited.insert({ pos_x, pos_y });
	global_visited.insert({ pos_x, pos_y });

	while (!q.empty()) {
		std::tuple<int, int> curr_cell = q.front();

		q.pop();

		for (const auto& dir : dirs) {
			std::tuple<int, int> new_cell = { std::get<0>(curr_cell) + std::get<0>(dir), std::get<1>(curr_cell) + std::get<1>(dir) };

			int new_row = std::get<0>(new_cell);
			int new_col = std::get<1>(new_cell);

			if (new_row >= 0 && new_row < rows && new_col >= 0 && new_col < cols && local_visited.count(new_cell) == 0 && global_visited.count(new_cell) == 0 && input[new_row][new_col] == input[pos_x][pos_y]) {
				local_visited.insert(new_cell);
				global_visited.insert(new_cell);

				q.push(new_cell);
			}
		}
	}

	int area = (int)local_visited.size();
	
	int perim = calc_perim(local_visited, input);

	return area * perim;
}

int calcualte_region_price2(const Grid& input, int pos_x, int pos_y, std::unordered_set<std::tuple<int, int>, TupleHash>& global_visited) {
	int rows = (int)input.size();
	int cols = (int)input[0].size();

	std::vector<std::tuple<int, int>> dirs = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

	std::unordered_set<std::tuple<int, int>, TupleHash> local_visited = {};

	std::queue<std::tuple<int, int>> q = {};

	q.push({ pos_x, pos_y });

	local_visited.insert({ pos_x, pos_y });
	global_visited.insert({ pos_x, pos_y });

	while (!q.empty()) {
		std::tuple<int, int> curr_cell = q.front();

		q.pop();

		for (const auto& dir : dirs) {
			std::tuple<int, int> new_cell = { std::get<0>(curr_cell) + std::get<0>(dir), std::get<1>(curr_cell) + std::get<1>(dir) };

			int new_row = std::get<0>(new_cell);
			int new_col = std::get<1>(new_cell);

			if (new_row >= 0 && new_row < rows && new_col >= 0 && new_col < cols && local_visited.count(new_cell) == 0 && global_visited.count(new_cell) == 0 && input[new_row][new_col] == input[pos_x][pos_y]) {
				local_visited.insert(new_cell);
				global_visited.insert(new_cell);

				q.push(new_cell);
			}
		}
	}

	int area = (int)local_visited.size();

	int perim = calc_perim2(local_visited, input);

	return area * perim;
}

int part1(Grid& input) {
	std::unordered_set<char> ch_set = {};

	for (const auto& row : input) {
		for (const auto& ch : row) {
			ch_set.insert(ch);
		}
	}
	std::unordered_set<std::tuple<int, int>, TupleHash> visited = {};
	int total = 0;
	for (const auto& ch : ch_set) {
		for (int i = 0; i < input.size(); ++i) {
			for (int j = 0; j < input[i].size(); ++j) {
				if (input[i][j] == ch && visited.count({i, j}) == 0) {
					int price = calcualte_region_price(input, i, j, visited);
					total += price;
				}
			}
		}
	}
	return total;
}


int part2(Grid& input) {
	std::unordered_set<char> ch_set = {};

	for (const auto& row : input) {
		for (const auto& ch : row) {
			ch_set.insert(ch);
		}
	}
	std::unordered_set<std::tuple<int, int>, TupleHash> visited = {};
	int total = 0;
	for (const auto& ch : ch_set) {
		for (int i = 0; i < input.size(); ++i) {
			for (int j = 0; j < input[i].size(); ++j) {
				if (input[i][j] == ch && visited.count({ i, j }) == 0) {
					int price = calcualte_region_price2(input, i, j, visited);
					total += price;
				}
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