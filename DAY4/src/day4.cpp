#include <iostream>
#include <string>
#include <fstream>
#include <cstring> // For std::strerror
#include <cerrno>  // For errno
#include <vector>
#include <sstream>
#include <stdexcept>
#include <regex>


const char* INPUT_FILE_PATH = "..\\..\\DAY4\\input\\input.txt";


std::vector<std::vector<char>> read_input(const char* file_path) {
	std::vector<std::vector<char>> input;

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


int part1(std::vector<std::vector<char>> input) {
	int result = 0;

	std::string target = "XMAS";

	for (int i = 0; i < input.size(); ++i) {
		for (int j = 0; j < input[i].size(); ++j) {
			if (input[i][j] == 'X') {
				// Search horizontally (left to right)
				int it = 0;
				for (int k = j; k < input[i].size(); ++k) {
					if (input[i][k] == target[it]) {
						it++;
					}
					else {
						break;
					}
					if (it == target.size()) {
						result++;
						break;
					}
				}
				// Search horizontally (right to left)
				it = 0;
				for (int k = j; k >= 0; --k) {
					if (input[i][k] == target[it]) {
						it++;
					}
					else {
						break;
					}
					if (it == target.size()) {
						result++;
						break;
					}
				}

				// Search vertically (up to bottom)
				it = 0;
				for (int k = i; k < input.size(); ++k) {
					if (input[k][j] == target[it]) {
						it++;
					}
					else {
						break;
					}
					if (it == target.size()) {
						result++;
						break;
					}
				}

				// Search vertically (bottom to up)
				it = 0;
				for (int k = i; k >= 0; --k) {
					if (input[k][j] == target[it]) {
						it++;
					}
					else {
						break;
					}
					if (it == target.size()) {
						result++;
						break;
					}
				}

				// Search diagonally
				it = 0;
				int d1 = i;
				int d2 = j;
				while (d1 < input.size() && d2 < input[i].size()) {
					if (input[d1][d2] == target[it]) {
						it++;
					}
					else {
						break;
					}

					if (it == target.size()) {
						result++;
						break;
					}

					d1++;
					d2++;
				}

				it = 0;
				d1 = i;
				d2 = j;
				while (d1 >= 0 && d2 >= 0) {
					if (input[d1][d2] == target[it]) {
						it++;
					}
					else {
						break;
					}

					if (it == target.size()) {
						result++;
						break;
					}

					d1--;
					d2--;
				}

				it = 0;
				d1 = i;
				d2 = j;
				while (d1 < input.size() && d2 >= 0) {
					if (input[d1][d2] == target[it]) {
						it++;
					}
					else {
						break;
					}

					if (it == target.size()) {
						result++;
						break;
					}

					d1++;
					d2--;
				}


				it = 0;
				d1 = i;
				d2 = j;
				while (d1 >= 0 && d2 < input[i].size()) {
					if (input[d1][d2] == target[it]) {
						it++;
					}
					else {
						break;
					}

					if (it == target.size()) {
						result++;
						break;
					}

					d1--;
					d2++;
				}


			}
		}
	}

	return result;
}

int part2(std::vector<std::vector<char>> input) {
	int result =0;

	for (int i = 0; i < input.size()-2; ++i) {
		for (int j = 0; j < input[i].size()-2; ++j) {
			if (
				((input[i][j] == 'M' && input[i+1][j+1] == 'A' && input[i+2][j+2] == 'S') 
				|| (input[i][j] == 'S' && input[i + 1][j + 1] == 'A' && input[i+2][j+2] == 'M'))
				&&
				((input[i][j+2] == 'M' && input[i + 1][j + 1] == 'A' && input[i+2][j] == 'S')
					|| (input[i][j+2] == 'S' && input[i + 1][j + 1] == 'A' && input[i+2][j] == 'M'))
				) {
				result++;
			}
		}
	}


	return result;
}


int main() {
	std::vector<std::vector<char>> input = read_input(INPUT_FILE_PATH);

	int part1_answer = part1(input);

	std::cout << "PART 1: " << part1_answer << std::endl;

	int part2_answer = part2(input);

	std::cout << "PART 2: " << part2_answer << std::endl;

	

	return 0;
}