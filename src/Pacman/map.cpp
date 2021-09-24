#include "Pacman/map.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

namespace Pacman {
	Position& Position::operator+=(const Dimensions& rhs) {
		y += rhs.height;
		x += rhs.width;

		return *this;
	}

	Position& Position::operator%=(const Dimensions& rhs) {
		y %= rhs.height;
		x %= rhs.width;

		return *this;
	}

	std::ostream& operator<<(std::ostream& stream, const Position& position) {
		stream << '(' << position.x << ',' << position.y << ')';
		return stream;
	}

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

	Dimensions Map::get_dimensions() const {
		u16 height = fields.size();
		u16 width = height > 0 ? fields[0].size() : 0;

		return { height, width };
	}

	Field& Map::get_field(Position position) {
		return fields[position.y][position.x];
	}
}