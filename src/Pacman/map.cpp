#include "Pacman/map.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

namespace Pacman {
	Map::Map(const std::string& file_name) {
		std::ifstream file(file_name);

		while(true) {
			std::string line;
			if(!std::getline(file, line))
				break;

			std::vector<Field> new_row;
			new_row.reserve(line.size());
			std::transform(line.cbegin(), line.cend(), std::back_inserter(new_row), [](char field) { return static_cast<Field>(field); });

			fields.push_back(std::move(new_row));
		}

		file.close();
	}

	void Map::print() const {
		std::stringstream output;

		for(const auto& row : fields) {
			for(const auto& field : row) {
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

				switch(field) {
				case Field::FOOD:
				case Field::ABILITY:
				case Field::ENEMY:
				case Field::PLAYER:
					output << NO_COLOR;
				}
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
}