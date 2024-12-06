#include <iostream>
#include <string>
#include <fstream>
#include <cstring> // For std::strerror
#include <cerrno>  // For errno
#include <vector>
#include <sstream>
#include <stdexcept>


const char* INPUT_FILE_PATH = "..\\..\\DAY2\\input\\input.txt";


std::vector<std::vector<int>> read_input(const char* file_path) {
	std::vector<std::vector<int>> input;

	std::ifstream file(file_path);
	if (!file) {
		char buffer[256]; // Buffer to store the error message
		strerror_s(buffer, sizeof(buffer), errno);
		std::cerr << "Unable to open file: " << buffer << '\n';
		throw std::runtime_error("Failed to open file");
	}

	std::string line;
	while (std::getline(file, line)) {
		std::string num;
		std::istringstream iss(line);
		std::vector<int> report;
		while (iss >> num) { // Extract words separated by whitespace
			report.emplace_back(std::stoi(num));

		}

		input.emplace_back(report);
	}

	file.close();


	return input;
}


int check_safety(std::vector<int> report) {

	bool increasing = false;
	bool decreasing = false;
	for (int i = 0; i < report.size()-1; ++i) {

		int diff = report[i] - report[i + 1];

		if (!(std::abs(diff) >= 1 && std::abs(diff) <= 3)) {
			return 0;
		}

		if (diff < 0) {
			increasing = true;
		}
		else if (diff > 0) {
			decreasing = true;
		}
		else {
			return 0;
		}
	}

	if ((increasing && !decreasing) || (decreasing && !increasing)) {
		return 1;
	}
	else {
		return 0;
	}
}

int check_safety_with_tolerance(std::vector<int> report) {

	int result = 0;
	std::vector<int> copy(report);
	if (check_safety(copy) == 1) {
		result++;
	}
	else {
		for (int i = 0; i < report.size(); ++i) {
			std::vector<int> copy(report);
			copy.erase(copy.begin() + i);
			if (check_safety(copy) == 1) {
				result++;
				break;
			}
		}
	}

	return result;
	
	return 0;
	
}


int part1(std::vector<std::vector<int>> input) {
	int result = 0;

	for (int i = 0; i < input.size(); ++i) {

		result += check_safety(input[i]);
	}

	return result;
}

int part2(std::vector<std::vector<int>> input) {
	int result = 0;

	for (int i = 0; i < input.size(); ++i) {
		//std::cout << "report " << i << " ";
		result += check_safety_with_tolerance(input[i]);
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