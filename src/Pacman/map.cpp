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

			if(fields.size() == 0)
				row_size = line.length();

			for(auto raw_field : line)
				fields.push_back(static_cast<Field>(raw_field));
		}

		file.close();
	}

	Dimensions Map::get_dimensions() const {
		u16 height = fields.size() / row_size;
		u16 width = row_size;

		return { height, width };
	}

	Field Map::get_field(Position position) const {
		return fields[position.y * row_size + position.x];
	}

	void Map::set_field(Position position, Field new_field) {
		fields[position.y * row_size + position.x] = new_field;
	}
}