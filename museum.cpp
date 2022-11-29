#pragma once
#include "museum.h"

Museum make_museum_form_csv(const std::string& fileName)
{
	if (auto ifstm = std::ifstream{ fileName }) {
		std::string line;
		if (std::getline(ifstm, line)) {
			std::string item;
			auto stream_line = std::istringstream(line);
			if (std::getline(stream_line, item, ',')) {
				const int width = std::atoi(item.c_str());
				if (std::getline(stream_line, item, ',')) {
					const int height = std::atoi(item.c_str());
					std::vector<std::vector<char>> data(height, std::vector<char>(width));
					for (int y = 0; y < height; ++y) {
						if (std::getline(ifstm, line)) {
							stream_line = std::istringstream(line);
							for (int x = 0; x < width; ++x) 
								if (std::getline(stream_line, item, ',')) 
									data[y][x] = item[0];
						}
					}
					return { width, height, data };
				}
			}
		}
	}
	throw std::runtime_error("file error!");
}
void output_museum_to_csv(const Museum& museum, const std::string fileName)
{
	if (auto ofstm = std::ofstream{ fileName }) {
		for (int x = 0; x < museum.width; ++x) {
			if (x == 0)
				ofstm << museum.width << ',';
			else if (x == 1)
				ofstm << museum.height << ',';
			else
				ofstm << ',';
		}
		ofstm << '\n';
		for (int y = 0; y < museum.height; ++y) {
			for (int x = 0; x < museum.width; ++x) {
				if (museum.data[y][x] != '\0')
					ofstm << museum.data[y][x];
				ofstm << ',';
			}
			ofstm << '\n';
		}
	}
}