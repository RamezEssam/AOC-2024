#include <iostream>
#include <string>
#include <fstream>
#include <cstring> // For std::strerror
#include <cerrno>  // For errno
#include <vector>
#include <sstream>
#include <stdexcept>
#include <unordered_map>



const char* INPUT_FILE_PATH = "..\\..\\DAY11\\input\\input.txt";



std::vector<long long> read_input(const char* file_path) {

	std::ifstream file(file_path);
	if (!file) {
		char buffer[256]; // Buffer to store the error message
		strerror_s(buffer, sizeof(buffer), errno);
		std::cerr << "Unable to open file: " << buffer << '\n';
		throw std::runtime_error("Failed to open file");
	}

	std::vector<long long> input;
	std::string token;
	std::stringstream buffer;
	buffer << file.rdbuf(); // Read the entire file into the buffer
	std::string content = buffer.str(); // Get the string from the buffer

	std::istringstream stream(content);
	while (stream >> token) {
		input.emplace_back(std::stoll(token));

	}

	file.close();

	return input;
}

int countDigits(long long number) {
	if (number == 0) return 1; // Special case for 0
	number = std::abs(number); // Handle negative numbers
	int count = 0;
	while (number > 0) {
		number /= 10;
		count++;
	}
	return count;
}

std::vector<long long> apply_rules(std::vector<long long>& input) {
	std::vector<long long> new_input = {};

	for (const auto& num : input) {
		if (num == 0) {
			new_input.emplace_back(1LL);
		}
		else if (countDigits(num) % 2 == 0) {
			std::string num_str = std::to_string(num);
			size_t midpoint = num_str.length() / 2;
			long long left_part = std::stoll(num_str.substr(0, midpoint));
			long long right_part = std::stoll(num_str.substr(midpoint));
			new_input.emplace_back(left_part);
			new_input.emplace_back(right_part);
		}
		else {
			new_input.emplace_back(num * 2024LL);
		}
	}

	return new_input;
}




size_t part1(std::vector<long long> input) {
	for (int i = 0; i < 25; ++i) {
		input = apply_rules(input);
	}
	

	return input.size();
}

long long part2(std::vector<long long>& input) {
	std::unordered_map<long long, long long> map = {};

	for (const auto& num : input) {
		if (map.count(num) > 0) {
			map[num]++;
		}
		else {
			map[num] = 1LL;
		}
	}

	for (int i = 0; i < 75; ++i) {
		std::unordered_map<long long, long long> new_map = {};
		for (auto& item : map) {
			std::vector<long long> new_stones = { item.first };
			for (auto& new_stone : apply_rules(new_stones)) {
				new_map[new_stone] += item.second;
			}
		}
		map = new_map;
	}

	long long total = 0;
	for (const auto& pair : map) {
		total += pair.second;
	};

	return total;
}


int main() {
	std::vector<long long> input = read_input(INPUT_FILE_PATH);

	size_t part1_answer = part1(input);

	std::cout << "PART 1: " << part1_answer << "\n";

	long long part2_answer = part2(input);

	std::cout << "PART 2: " << part2_answer << "\n";
	
	return 0;
}