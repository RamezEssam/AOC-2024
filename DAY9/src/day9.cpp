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
#include <algorithm> // For std::swap

const char* INPUT_FILE_PATH = "..\\..\\DAY9\\input\\input.txt";

std::vector<std::string> read_input(const char* file_path) {

	std::ifstream file(file_path);
	if (!file) {
		char buffer[256]; // Buffer to store the error message
		strerror_s(buffer, sizeof(buffer), errno);
		std::cerr << "Unable to open file: " << buffer << '\n';
		throw std::runtime_error("Failed to open file");
	}

	std::vector<char> input;
	char ch;
	while (file.get(ch)) {
		if (ch != '\n') {
			input.emplace_back(ch);
		}
		
	}

	file.close();

	std::vector<std::string> parsed_input = {};
	bool file_flag = true;
	int file_block_id = 0;
	for (int i = 0; i < input.size(); ++i) {

		if (file_flag) {
			
			for (int j = 0; j < (input[i] - '0'); ++j) {

				parsed_input.emplace_back(std::to_string(file_block_id));
			}
			file_flag = false;
			file_block_id++;
		}
		else {
			for (int j = 0; j < (input[i] - '0'); ++j) {
				parsed_input.emplace_back(".");
			}
			file_flag = true;
		}
	}

	return parsed_input;
}

long long part1(std::vector<std::string> input) {
	int i = 0;
	int j = (int)input.size() - 1;
	int n = (int)input.size();

	while (i < n && j >= 0) {
		if (i > j) {
			break;
		}

		if (input[i] == "." && input[j] != ".") {
			std::swap(input[i], input[j]);
		}

		if (input[i] != ".") {
			i++;
		}

		if (input[j] == ".") {
			j -= 1;
		}
	}
	long long total = 0LL;

	for (int i = 0; i < n; ++i) {
		if (input[i] != ".") {
			total += (std::stoi(input[i])) * i;
		}
	}

	return total;
}


void swap_ranges(std::tuple<int, int>& r1, std::tuple<int, int>& r2, std::vector<std::string>& arr) {
	if (std::get<1>(r1) - std::get<0>(r1) + 1 >= std::get<1>(r2) - std::get<0>(r2) + 1) {
		int i = std::get<0>(r1);
		int j = std::get<0>(r2);

		while (i <= std::get<1>(r1) && j <= std::get<1>(r2)) {
			if (arr[i] == ".") {
				std::swap(arr[i], arr[j]);
			}
			i++;
			j++;
		}
	}
}

long long part2(std::vector<std::string> input) {
	int start = 0;
	std::vector<std::tuple<int, int>> ranges = {};

	for (int i = 1; i < input.size(); ++i) {
		if (input[i] != input[i - 1]) {
			if (input[i - 1] != ".") {
				ranges.emplace_back(std::make_tuple(start, i-1));
			}
			start = i;
		}
	}
	if (input.back() != ".") {
		ranges.emplace_back(std::make_tuple(start, (int)input.size() - 1));
	}

	for (int i = (int)ranges.size() - 1; i >= 0; --i) {
		int rng_size = std::get<1>(ranges[i]) - std::get<0>(ranges[i]) + 1;
		bool free_space = false;
		int start = 0;
		for (int j = 0; j < input.size(); ++j) {
			if (input[j] == ".") {
				if (!free_space) {
					start = j;
					free_space = true;
				}
			}
			else {
				if (free_space) {
					std::tuple<int, int> free_space_rng = { start, j - 1 };
					if (std::get<1>(free_space_rng) - std::get<0>(free_space_rng) + 1 >= rng_size && std::get<0>(free_space_rng) < std::get<0>(ranges[i]) && std::get<1>(free_space_rng) < std::get<1>(ranges[i])) {
						swap_ranges(free_space_rng, ranges[i], input);
						/*for (const auto& s : input) {
							std::cout << s;
						}
						std::cout << std::endl;*/
						break;
					}
					free_space = false;
				}
			}
		}
		if (free_space) {
			std::tuple<int, int> free_space_rng = { start, (int)input.size() - 1};
			if (std::get<1>(free_space_rng) - std::get<0>(free_space_rng) + 1 >= rng_size && std::get<0>(free_space_rng) < std::get<0>(ranges[i]) && std::get<1>(free_space_rng) < std::get<1>(ranges[i])) {
				swap_ranges(free_space_rng, ranges[i], input);
				/*for (const auto& s : input) {
					std::cout << s;
				}
				std::cout << std::endl;*/
			}
		}
	}

	

	long long total = 0;
	for (int i = 0; i < input.size(); ++i) {
		if (input[i] != "." ){
			total += std::stoi(input[i]) * i;
		}
	}
	

	return total;
}


int main() {
	std::vector<std::string> input = read_input(INPUT_FILE_PATH);

	long long part1_answer = part1(input);

	std::cout << "PART 1: " << part1_answer << "\n";

	long long part2_answer = part2(input);
	
	std::cout << "PART 2: " << part2_answer << "\n";

	return 0;
}