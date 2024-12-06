#include <iostream>
#include <string>
#include <fstream>
#include <cstring> // For std::strerror
#include <cerrno>  // For errno
#include <vector>
#include <sstream>
#include <utility>  // For std::move
#include <stdexcept>
#include <algorithm>  // For std::sort


const char* INPUT_FILE_PATH = "..\\..\\DAY1\\input\\input.txt";


std::vector<std::vector<int>> read_input(const char* file_path) {
	std::vector<std::vector<int>> input;

	std::ifstream file(file_path);
	if (!file) {
		char buffer[256]; // Buffer to store the error message
		strerror_s(buffer, sizeof(buffer), errno);
		std::cerr << "Unable to open file: " << buffer << '\n';
		throw std::runtime_error("Failed to open file");
	}

	std::vector<int> left;
	std::vector<int> right;

	std::string line;
	while (std::getline(file, line)) {
		std::string num;
		std::istringstream iss(line);
		bool push_left = true;
		while (iss >> num) { // Extract words separated by whitespace
			if (push_left) {
				left.emplace_back(std::stoi(num));
				push_left = false;
			}
			else {
				right.emplace_back(std::stoi(num));
			}

		}
	}

	file.close();

	input.emplace_back(left);
	input.emplace_back(right);

	return input;
}


int part1(std::vector<std::vector<int>> input) {

	std::vector<int> left = input[0];
	std::vector<int> right = input[1];

	std::sort(left.begin(), left.end());
	std::sort(right.begin(), right.end());

	int result = 0;

	for (int i = 0; i < left.size(); ++i) {
		result += std::abs((left[i] - right[i]));
	}

	return result;
}

int part2(std::vector<std::vector<int>> input) {

	std::vector<int> left = input[0];
	std::vector<int> right = input[1];

	int result = 0;

	for (int i = 0; i < left.size(); ++i) {
		int similarity_score = 0;
		for (int j = 0; j < right.size(); ++j) {
			if (right[j] == left[i]) similarity_score++;
		}

		result += left[i] * similarity_score;
	}

	return result;
}


int main() {

	std::vector<std::vector<int>> input = read_input(INPUT_FILE_PATH);

	int part1_answer = part1(input);

	int part2_answer = part2(input);

	std::cout << "PART 1: " << part1_answer << std::endl;

	std::cout << "PART 2: " << part2_answer << std::endl;

	return 0;
}
