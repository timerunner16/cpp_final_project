#include <vector>
#include <string>

std::vector<std::string> split_string(std::string input, std::string delimiter);
std::string trim_whitespace(std::string input, std::string whitespace = " \t");
std::string trim_trailing_comment(std::string input, std::string comment= "//");
std::string trim_last(std::string input);
std::string trim_double(std::string input, char character = ' ');
std::string trim_nonalphanum(std::string input);
