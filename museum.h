#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>

#pragma comment(lib, "gdiplus")

struct Museum {
	int width;
	int height;
	std::vector<std::vector<char>> data;
};

Museum make_museum_form_csv(const std::string& fileName);
void output_museum_to_csv(const Museum& museum, const std::string fileName);