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
#include <cmath>



const char* INPUT_FILE_PATH = "..\\..\\DAY13\\input\\input.txt";

struct GameState {
	std::tuple<int, int> btn_a;
	std::tuple<int, int> btn_b;
	std::tuple<long long, long long> prize;
};


std::vector<GameState> read_input(const char* file_path) {

	std::vector<GameState> input = {};

	std::ifstream file(file_path);
	if (!file) {
		char buffer[256]; // Buffer to store the error message
		strerror_s(buffer, sizeof(buffer), errno);
		std::cerr << "Unable to open file: " << buffer << '\n';
		throw std::runtime_error("Failed to open file");
	}

	// Read lines from the file
	std::string line;
	GameState state = {};
	while (std::getline(file, line)) {
		
		// Process the line
		if (line.find("Button A") == 0) {
			int dx = 0;
			int dy = 0;

			size_t posX = line.find("X+");
			size_t posY = line.find("Y+");

			dx = std::stoi(line.substr(posX + 2));
			dy = std::stoi(line.substr(posY + 2));
			state.btn_a = { dx, dy };
		}
		else if (line.find("Button B") == 0) {
			int dx = 0;
			int dy = 0;

			size_t posX = line.find("X+");
			size_t posY = line.find("Y+");

			dx = std::stoi(line.substr(posX + 2));
			dy = std::stoi(line.substr(posY + 2));
			state.btn_b = { dx, dy };
		}
		else if(line.find("Prize") == 0) {
			long long dx = 0;
			long long dy = 0;

			size_t posX = line.find("X=");
			size_t posY = line.find("Y=");

			dx = std::stoll(line.substr(posX + 2));
			dy = std::stoll(line.substr(posY + 2));
			state.prize = { dx, dy };

			input.emplace_back(state);
		}
		
		
	}

	file.close();

	return input;
}

long long solve_game_state(GameState& state) {
	int costA = 3;
	int costB = 1;
	long long a = (long long)std::get<0>(state.btn_a);
	long long b = (long long)std::get<0>(state.btn_b);
	long long d = (long long)std::get<1>(state.btn_a);
	long long e = (long long)std::get<1>(state.btn_b);
	long long c1 = std::get<0>(state.prize);
	long long c3 = std::get<1>(state.prize);

	if ((a * e) - (b * d) == 0) {
		return 0LL;
	}
	long double x1 = (static_cast<long double>(c1) * e - b * c3) / ((a * e) - (b * d));
	long double x2 = (static_cast<long double>(a) * c3 - d * c1) / ((a * e) - (b * d));

	if (x1 == std::floor(x1) && x2 == std::floor(x2)) {
		return (long long)x1 * costA + (long long)x2 * costB;
	}
	else {
		return 0LL;
	}

}

long long part1(std::vector<GameState>& input) {

	long long total = 0;

	for (GameState& state : input) {
		total += solve_game_state(state);
	}

	return total;
}

long long part2(std::vector<GameState>& input) {
	long long total = 0;
	for (GameState& state : input) {
		std::tuple<long long, long long> new_prize = { std::get<0>(state.prize) + 10000000000000LL , std::get<1>(state.prize) + 10000000000000LL };
		state.prize = new_prize;
		total += solve_game_state(state);;
	}

	return total;

}


int main() {
	std::vector<GameState> input = read_input(INPUT_FILE_PATH);

	long long part1_answer = part1(input);

	std::cout << "PART 1: " << part1_answer << "\n";

	long long part2_answer = part2(input);

	std::cout << "PART 2: " << part2_answer << "\n";
	
	return 0;
}