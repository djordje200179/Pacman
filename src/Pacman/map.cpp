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

	Map::Map(const std::string& fileName) {
		auto file = std::ifstream(fileName);

		while(true) {
			static auto line = std::string();
			if(!std::getline(file, line))
				break;

			if(fields.size() == 0)
				rowSize = line.length();

			for(auto rawField : line)
				fields.push_back(static_cast<Field>(rawField));
		}

		file.close();
	}

	Dimensions Map::getDimensions() const {
		u16 height = fields.size() / rowSize;
		u16 width = rowSize;

		return { height, width };
	}

	Field Map::getField(Position position) const {
		return fields[position.y * rowSize + position.x];
	}

	void Map::setField(Position position, Field newField) {
		fields[position.y * rowSize + position.x] = newField;
	}
}