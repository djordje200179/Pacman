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
		auto file = std::ifstream(file_name);

		while(true) {
			static auto line = std::string();
			if(!std::getline(file, line))
				break;

			static std::vector<Field> new_row;
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

	Field Map::get_field(Position position) const {
		return fields[position.y][position.x];
	}

	void Map::set_field(Position position, Field new_field) {
		fields[position.y][position.x] = new_field;
	}
}