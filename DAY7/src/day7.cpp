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


const char* INPUT_FILE_PATH = "..\\..\\DAY7\\input\\input.txt";


struct Equation {
    long long result;
    std::vector<int> inputs;
};

// Custom hash function for std::tuple<int, long long>
struct TupleHash {
    size_t operator()(const std::tuple<int, long long>& t) const {
        size_t hash1 = std::hash<int>{}(std::get<0>(t));       // Hash the int part
        size_t hash2 = std::hash<long long>{}(std::get<1>(t));  // Hash the long long part
        return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2)); // Combine the hashes
    }
};


std::vector<Equation> read_input(const char* file_path) {
    std::vector<Equation> input;
    std::ifstream file(file_path);
    if (!file) {
        char buffer[256]; // Buffer to store the error message
        strerror_s(buffer, sizeof(buffer), errno);
        std::cerr << "Unable to open file: " << buffer << '\n';
        throw std::runtime_error("Failed to open file");
    }

    std::string line;
    while (std::getline(file, line)) {
        Equation eq;
        std::istringstream Eqstream(line);
        char Eqdelimiter = ':';

        std::string result;
        std::string inputs;

        std::getline(Eqstream, result, Eqdelimiter);
        std::getline(Eqstream, inputs, Eqdelimiter);

        eq.result = std::stoll(result);

        std::istringstream instream(inputs);
        char indelimiter = ' ';
        std::string in;
        std::vector<int> ins;
        while (std::getline(instream, in, indelimiter)) {
            try {
                ins.emplace_back(std::stoi(in));
            }
            catch (const std::invalid_argument& e) {
                continue;
            }
        }
        eq.inputs = ins;

        input.emplace_back(eq);
    }

    file.close();

    return input;
}


bool can_solve1(const std::vector<int>& nums, long long target, int index, long long current,
    std::unordered_map<std::tuple<int, long long>, bool, TupleHash>& memo) {

    std::tuple<int, long long> state(index, current);

    if (memo.count(state)) {
        return memo[state];
    }

    if (index == nums.size()) {
        return current == target;
    }

    bool add_result = can_solve1(nums, target, index + 1, current + nums[index], memo);
    bool mul_result = can_solve1(nums, target, index + 1, (current == 0 ? 1 : current) * nums[index], memo);

    memo[state] = add_result || mul_result;
    return memo[state];
}


bool can_solve2(const std::vector<int>& nums, long long target, int index, long long current,
    std::unordered_map<std::tuple<int, long long>, bool, TupleHash>& memo) {

    std::tuple<int, long long> state(index, current);

    if (memo.count(state)) {
        return memo[state];
    }

    if (index == nums.size()) {
        return current == target;
    }

    bool add_result = can_solve2(nums, target, index + 1, current + nums[index], memo);
    bool mul_result = can_solve2(nums, target, index + 1, (current == 0 ? 1 : current) * nums[index], memo);
    long long concat_val = std::stoll(current != 0 ? std::to_string(current) + std::to_string(nums[index]) : std::to_string(nums[index]));
    bool concat_result = can_solve2(nums, target, index + 1, concat_val, memo);

    memo[state] = add_result || mul_result || concat_result;
    return memo[state];
}


long long check_equation1(const Equation& eq) {
    std::unordered_map<std::tuple<int, long long>, bool, TupleHash> memo;
    if (can_solve1(eq.inputs, eq.result, 0, 0, memo)) {
        return eq.result;
    }
    else {
        return 0;
    }
}

long long check_equation2(const Equation& eq) {
    std::unordered_map<std::tuple<int, long long>, bool, TupleHash> memo;
    if (can_solve2(eq.inputs, eq.result, 0, 0, memo)) {
        return eq.result;
    }
    else {
        return 0;
    }
}

long long part1(std::vector<Equation> input) {
    long long result = 0;
    for (const auto& eq : input) {
        result += check_equation1(eq);
    }
    return result;
}

long long part2(std::vector<Equation> input) {
    long long result = 0;
    for (const auto& eq : input) {
        result += check_equation2(eq);
    }
    return result;
}

int main() {
    std::vector<Equation> input = read_input(INPUT_FILE_PATH);

    long long part1_answer = part1(input);

    std::cout << "PART 1: " << part1_answer << std::endl;

    long long part2_answer = part2(input);

    std::cout << "PART 2: " << part2_answer << std::endl;

    return 0;
}
