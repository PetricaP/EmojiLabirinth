#pragma once

#include <vector>
#include <fstream>

inline std::vector<char> load_file(const std::string &filePath) {
	std::ifstream file(filePath, std::ios::binary);
	return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}
