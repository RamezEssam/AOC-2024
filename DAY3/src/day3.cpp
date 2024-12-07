#include <iostream>
#include <string>
#include <fstream>
#include <cstring> // For std::strerror
#include <cerrno>  // For errno
#include <vector>
#include <sstream>
#include <stdexcept>
#include <regex>


const char* INPUT_FILE_PATH = "..\\..\\DAY3\\input\\input.txt";


std::string read_input(const char* file_path) {
	std::string input;

	std::ifstream file(file_path);
	if (!file) {
		char buffer[256]; // Buffer to store the error message
		strerror_s(buffer, sizeof(buffer), errno);
		std::cerr << "Unable to open file: " << buffer << '\n';
		throw std::runtime_error("Failed to open file");
	}

	// Read the file into a string
	std::ostringstream buffer;
	buffer << file.rdbuf(); // Read the file stream into the buffer
	input = buffer.str(); // Convert buffer to string

	file.close();


	return input;
}

int part1(std::string input) {
	int result = 0;

	std::regex pattern("mul\\(\\d{1,3},\\d{1,3}\\)");
	std::regex numPattern("\\d{1,3}");

	auto matches_begin = std::sregex_iterator(input.begin(), input.end(), pattern);
	auto matches_end = std::sregex_iterator();

	for (std::sregex_iterator i = matches_begin; i != matches_end; ++i) {
		std::smatch match = *i;
		std::string operation = match.str();
		matches_begin = std::sregex_iterator(operation.begin(), operation.end(), numPattern);
		matches_end = std::sregex_iterator();
		int mul_result = 1;
		for (std::sregex_iterator i = matches_begin; i != matches_end; ++i) {
			match = *i;
			mul_result *= std::stoi(match.str());
		}
		result += mul_result;

	}

	return result;
}

int part2(std::string input) {
	int result = 0;
	bool enabled = true;

	std::regex pattern("(mul\\(\\d{1,3},\\d{1,3}\\))|(do\\(\\)|don't\\(\\))");
	std::regex numPattern("\\d{1,3}");

	auto matches_begin = std::sregex_iterator(input.begin(), input.end(), pattern);
	auto matches_end = std::sregex_iterator();

	for (std::sregex_iterator i = matches_begin; i != matches_end; ++i) {
		std::smatch match = *i;
		if (match.str() == "do()") {
			enabled = true;
		}
		else if (match.str() == "don't()") {
			enabled = false;
		}
		else {
			if (enabled) {
				std::string operation = match.str();
				matches_begin = std::sregex_iterator(operation.begin(), operation.end(), numPattern);
				matches_end = std::sregex_iterator();
				int mul_result = 1;
				for (std::sregex_iterator i = matches_begin; i != matches_end; ++i) {
					match = *i;
					mul_result *= std::stoi(match.str());
				}
				result += mul_result;
			}
			
		}
	}


	return result;
}



int main() {
	std::string input = read_input(INPUT_FILE_PATH);

	int part1_answer = part1(input);

	int part2_answer = part2(input);

	std::cout << "PART 1: " << part1_answer << std::endl;
	std::cout << "PART 2: " << part2_answer << std::endl;

	return 0;
}