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
#include <regex>
#include <windows.h>



const char* INPUT_FILE_PATH = "..\\..\\DAY14\\input\\input.txt";

struct Robot {
	std::tuple<int, int> pos;
	std::tuple<int, int> velocity;
};

struct TupleHash {
	std::size_t operator()(const std::tuple<int, int>& t) const {
		std::size_t h1 = std::hash<int>{}(std::get<0>(t));
		std::size_t h2 = std::hash<int>{}(std::get<1>(t));

		// Combine the two hash values
		return h1 ^ (h2 << 1); // XOR with a bit shift for better distribution
	}
};


std::vector<Robot> read_input(const char* file_path) {

	std::vector<Robot> input = {};

	std::ifstream file(file_path);
	if (!file) {
		char buffer[256]; // Buffer to store the error message
		strerror_s(buffer, sizeof(buffer), errno);
		std::cerr << "Unable to open file: " << buffer << '\n';
		throw std::runtime_error("Failed to open file");
	}

	// Read lines from the file
	std::string line;
	Robot state = {};
	std::regex numberRegex("-?\\d+");
	while (std::getline(file, line)) {

		// Process the line
		std::vector<int> nums = {};
		std::sregex_iterator begin(line.begin(), line.end(), numberRegex);
		std::sregex_iterator end;

		for (auto &it = begin; it != end; ++it) {
			nums.emplace_back(std::stoi(it->str()));
		}

		state.pos = {nums[0], nums[1]};
		state.velocity = { nums[2], nums[3] };

		input.emplace_back(state);


	}

	file.close();

	return input;
}

static inline int mod(int a, int b) {
	return (a % b + b) % b;
}


void move_robots(std::vector<Robot>& input, std::unordered_map<std::tuple<int, int>, int, TupleHash>& locs) {
	for (auto& robot : input) {
		if (locs.count(robot.pos) != 0) {
			locs[robot.pos]--;
		}

		robot.pos = { mod(std::get<0>(robot.pos) + std::get<0>(robot.velocity) , 101), mod(std::get<1>(robot.pos) + std::get<1>(robot.velocity) , 103) };

		if (locs.count(robot.pos) == 0) {
			locs[robot.pos] = 1;
		}
		else {
			locs[robot.pos]++;
		}
	}
}

int part1(std::vector<Robot>& input) {
	std::unordered_map<std::tuple<int, int>, int, TupleHash> locs = {};
	for (const auto& robot : input) {
		if (locs.count(robot.pos) == 0) {
			locs[robot.pos] = 1;
		}
		else {
			locs[robot.pos]++;
		}
	}

	
	for (int i = 0; i < 100; ++i) {
		move_robots(input, locs);
	}
	int first_quad_count = 0;
	int second_quad_count = 0;
	int third_quad_count = 0;
	int fourth_quad_count = 0;
	for (const auto& pair : locs) {
		int robot_x_pos = std::get<0>(pair.first);
		int robot_y_pos = std::get<1>(pair.first);

		// first quadrant start_x = 0, end_x = 49, start_y = 0, end_y = 50
		if (robot_x_pos >= 0 && robot_x_pos <= 49 && robot_y_pos >= 0 && robot_y_pos <= 50) {
			first_quad_count += pair.second;
		}
		// second quadrant start_x = 51, end_x = 100, start_y = 0, end_y = 50
		else if (robot_x_pos >= 51 && robot_x_pos <= 100 && robot_y_pos >= 0 && robot_y_pos <= 50) {
			second_quad_count += pair.second;
		}
		// third quadrant start_x = 0, end_x = 49, start_y = 52, end_y = 102
		else if (robot_x_pos >= 0 && robot_x_pos <= 49 && robot_y_pos >= 52 && robot_y_pos <= 102) {
			third_quad_count += pair.second;
		}
		// fourth quadrant start_x = 51, end_x = 100, start_y = 52, end_y = 102
		else if (robot_x_pos >= 51 && robot_x_pos <= 100 && robot_y_pos >= 52 && robot_y_pos <= 102) {
			fourth_quad_count += pair.second;
		}
	}

	return first_quad_count * second_quad_count * third_quad_count * fourth_quad_count;
}


int part2(std::vector<Robot>& input) {
	std::unordered_map<std::tuple<int, int>, int, TupleHash> locs = {};
	for (const auto& robot : input) {
		if (locs.count(robot.pos) == 0) {
			locs[robot.pos] = 1;
		}
		else {
			locs[robot.pos]++;
		}
	}

	bool tree_located = false;
	int seconds = 0;
	while (!tree_located) {
		move_robots(input, locs);
		seconds++;
		bool all_ones = true;
		for (const auto& pair : locs) {
			if (pair.second > 1) {
				all_ones = false;
			}
		}
		if (all_ones) {
			tree_located = true;
		}
	}

	return seconds;
}


int main() {
	std::vector<Robot> input = read_input(INPUT_FILE_PATH);

	int part1_answer = part1(input);

	std::cout << "PART 1: " << part1_answer << "\n";

	int part2_answer = part2(input);

	std::cout << "PART 2: " << part2_answer << "\n";

	return 0;
}