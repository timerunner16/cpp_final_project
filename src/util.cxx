#include "util.hpp"

std::vector<std::string> split_string(std::string input, std::string delimiter) {
	std::vector<std::string> output = std::vector<std::string>();
	std::string remaining_string = input;
	int pos = remaining_string.find(delimiter);
	while (pos != std::string::npos) {
		std::string current = remaining_string.substr(0, pos);
		output.push_back(current);
		remaining_string.erase(0,pos+delimiter.size());
		pos = remaining_string.find(delimiter);
	}
	output.push_back(remaining_string);
	return output;
}

std::string trim_whitespace(std::string input, std::string whitespace) {
	size_t begin = input.find_first_not_of(whitespace);
	if (begin == std::string::npos) return "";
	size_t end = input.find_last_not_of(whitespace);
	size_t range = end-begin+1;
	return input.substr(begin, range);
}

std::string trim_trailing_comment(std::string input, std::string comment) {
	size_t pos = input.find(comment);
	if (pos == std::string::npos) return input;
	return input.substr(0,pos);
}

std::string trim_last(std::string input) {
	return input.substr(0,input.size()-2);
}

std::string trim_double(std::string input, char character) {
	while (true) {
		bool found = false;
		for (size_t i = 0; i < input.size();) {
			if (i < input.size()-1 && input[i] == character && input[i+1] == character) {
				input.erase(i, 1);
				found = true;
			}
			else i++;
		}
		if (!found) break;
	}
	return input;
}

std::string trim_nonstandard(std::string input) {
	while (true) {
		bool found = false;
		for (size_t i = 0; i < input.size();) {
			if (i < input.size() && !(isalpha(input[i]) || isdigit(input[i]) || ispunct(input[i]))) {
				input.erase(i, 1);
				found = true;
			}
			else i++;
		}
		if (!found) break;
	}
	return input;
}
