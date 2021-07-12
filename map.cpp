#include "map.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

Map::Map(const std::string& file_name) {
	auto file = std::ifstream(file_name);

	while(true) {
		auto line = std::string();
		if(!std::getline(file, line))
			break;

		auto new_row = std::vector<Field>();
		for(auto& field : line)
			new_row.push_back(static_cast<Field>(field));
		fields.push_back(new_row);
	}

	file.close();
}

void Map::print() const {
	auto output = std::stringstream();

	for(const auto& row: fields) {
		for(const auto& field: row) {
			switch(field) {
			case Field::FOOD:
			case Field::ABILITY:
				output << YELLOW_COLOR;
				break;
			case Field::ENEMY:
				output << RED_COLOR;
				break;
			case Field::PLAYER:
				output << GREEN_COLOR;
				break;
			}

			output << static_cast<char>(field);
			output << NO_COLOR;
		}

		output << '\n';
	}

	std::cout << output.str();
}

Map::Dimensions Map::get_dimensions() const {
	u16 height = fields.size();
	u16 width = height > 0 ? fields[0].size() : 0;

	return { height, width };
}

Map::Field& Map::get_field(Position position) {
	return fields[position.y][position.x];
}