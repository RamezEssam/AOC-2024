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
#include <algorithm>

const char* INPUT_FILE_PATH = "..\\..\\DAY5\\input\\input.txt";


typedef std::unordered_map<int, std::vector<int>> Rules;

typedef std::vector<std::vector<int>> PageUpdates;


std::tuple<Rules, PageUpdates> read_input(const char* file_path) {

	std::ifstream file(file_path);
	if (!file) {
		char buffer[256]; // Buffer to store the error message
		strerror_s(buffer, sizeof(buffer), errno);
		std::cerr << "Unable to open file: " << buffer << '\n';
		throw std::runtime_error("Failed to open file");
	}

	std::string line;
	bool readRules = true;
	Rules rules;
	PageUpdates page_updates;
	while (std::getline(file, line)) {
		if (line.empty()) {
			readRules = false;
			continue;
		}
		if (readRules) {
			std::istringstream stream(line);
			char delimiter = '|';
			std::string left;
			std::string right;

			std::getline(stream, left, delimiter);
			std::getline(stream, right, delimiter);

			int key = std::stoi(left);
			int val = std::stoi(right);

			if (rules.find(key) != rules.end()) {
				rules[key].emplace_back(val);
			}
			else {
				std::vector<int> vec = { val };
				rules[key] = vec;
			}

		}
		else {
			std::istringstream stream(line);
			char delimiter = ',';
			std::string page;
			std::vector<int> update;
			while (std::getline(stream, page, delimiter)) {
				update.emplace_back(std::stoi(page));
			}
			page_updates.emplace_back(update);
		}

	}

	file.close();

	std::tuple<Rules, PageUpdates> input = {rules, page_updates};

	return input;
}


int checkUpdateOrder(Rules rules, std::vector<int> update) {
	for (int i = 0; i < update.size()-1; ++i) {
		for (int j = i+1; j < update.size(); ++j) {
			int key = update[i];

			if (std::find(rules[key].begin(), rules[key].end(), update[j]) == rules[key].end()) {
				return 0;
			}
		}
	}

	return update[update.size()/2];
}


int part1(std::tuple<Rules, PageUpdates> input) {
	int result = 0;

	Rules rules = std::get<0>(input);
	PageUpdates updates = std::get<1>(input);
	

	for (int i = 0; i < updates.size(); ++i) {
		result += checkUpdateOrder(rules, updates[i]);
	}

	return result;
}


int sortUnorderedUpdates(std::vector<int> update, Rules rules) {
	std::vector<std::tuple<int, int>> pageCounts;
	for (int i = 0; i < update.size(); ++i) {
		int count = 0;
		for (int j = 0; j < update.size(); ++j) {
			if (i != j){
				for (int k = 0; k < rules[update[i]].size(); ++k) {
					if (rules[update[i]][k] == update[j]) {
						count++;
					}
				}
			}
			
		}
		pageCounts.emplace_back(std::make_tuple(update[i], count));
	}

	std::sort(pageCounts.begin(), pageCounts.end(), [](const auto& a, const auto& b) {
		return std::get<1>(a) > std::get<1>(b);
	});

	return std::get<0>(pageCounts[pageCounts.size() / 2]);
	
}


int part2(std::tuple<Rules, PageUpdates> input) {
	Rules rules = std::get<0>(input);
	PageUpdates updates = std::get<1>(input);
	int result = 0;
	for (int i = 0; i < updates.size(); ++i) {
		if (checkUpdateOrder(rules, updates[i]) == 0) {
			result += sortUnorderedUpdates(updates[i], rules);
		}
	}

	return result;

}

int main() {
	std::tuple<Rules, PageUpdates> input = read_input(INPUT_FILE_PATH);

	std::cout << "PART 1: " << part1(input) << std::endl;
	std::cout << "PART 2: " << part2(input) << std::endl;
	return 0;
}