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

const char* INPUT_FILE_PATH = "..\\..\\DAY8\\input\\input.txt";
//const char* INPUT_FILE_PATH = "..\\..\\DAY8\\input\\sample.txt";


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

int part1(Grid& input) {
	std::unordered_set<std::tuple<int, int>, TupleHash> antennas;
	std::unordered_set<std::tuple<int, int>, TupleHash> antinodes;
	for (int i = 0; i < input.size(); ++i) {
		for (int j = 0; j < input[i].size(); ++j) {
			if (input[i][j] != '.') {
				antennas.insert(std::make_tuple(i, j));
			}
		}
	}

	for (const auto& antena : antennas) {
		for (const auto& other : antennas) {
			if (antena != other) {
				if (input[std::get<0>(antena)][std::get<1>(antena)] == input[std::get<0>(other)][std::get<1>(other)]) {
					int rise = std::get<0>(antena) - std::get<0>(other);
					int run = std::get<1>(antena) - std::get<1>(other);
					std::tuple<int, int> an1 = std::make_tuple(std::get<0>(antena) + rise, std::get<1>(antena) + run);
					std::tuple<int, int> an2 = std::make_tuple(std::get<0>(other) - rise, std::get<1>(other) - run);

					if (std::get<0>(an1) >= 0 && std::get<0>(an1) < input.size() && std::get<1>(an1) >= 0 && std::get<1>(an1) < input[0].size()) {
						antinodes.insert(an1);
					}

					if (std::get<0>(an2) >= 0 && std::get<0>(an2) < input.size() && std::get<1>(an2) >= 0 && std::get<1>(an2) < input[0].size()) {
						antinodes.insert(an2);
					}
				}
			}
		}
	}
	return (int)antinodes.size();
}

int part2(Grid& input) {
	std::unordered_set<std::tuple<int, int>, TupleHash> antennas;
	std::unordered_set<std::tuple<int, int>, TupleHash> antinodes;
	for (int i = 0; i < input.size(); ++i) {
		for (int j = 0; j < input[i].size(); ++j) {
			if (input[i][j] != '.') {
				antennas.insert(std::make_tuple(i, j));
			}
		}
	}

	for (const auto& antena : antennas) {
		for (const auto& other : antennas) {
			if (antena != other) {
				if (input[std::get<0>(antena)][std::get<1>(antena)] == input[std::get<0>(other)][std::get<1>(other)]) {
					int rise = std::get<0>(antena) - std::get<0>(other);
					int run = std::get<1>(antena) - std::get<1>(other);
					std::tuple<int, int> an1 = std::make_tuple(std::get<0>(antena), std::get<1>(antena));
					std::tuple<int, int> an2 = std::make_tuple(std::get<0>(other) , std::get<1>(other));

					while (std::get<0>(an1) >= 0 && std::get<0>(an1) < input.size() && std::get<1>(an1) >= 0 && std::get<1>(an1) < input[0].size()) {
						antinodes.insert(an1);
						an1 = std::make_tuple(std::get<0>(an1) + rise, std::get<1>(an1) + run);
					}

					while (std::get<0>(an2) >= 0 && std::get<0>(an2) < input.size() && std::get<1>(an2) >= 0 && std::get<1>(an2) < input[0].size()) {
						antinodes.insert(an2);
						an2 = std::make_tuple(std::get<0>(an2) + rise, std::get<1>(an2) + run);
					}

				}
			}
		}
	}

	return (int)antinodes.size();
}


int main() {
	Grid input = read_input(INPUT_FILE_PATH);

	int part1_answer = part1(input);

	std::cout << "PART 1: " << part1_answer << "\n";

	int part2_answer = part2(input);

	std::cout << "PART 2: " << part2_answer << "\n";
}