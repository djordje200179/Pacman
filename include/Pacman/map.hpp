#pragma once

#include "misc.hpp"
#include <string>
#include <ostream>
#include <vector>

namespace Pacman {
	enum class Field : char {
		SPACE = ' ', WALL = '#', FOOD = '.', ABILITY = 'O', ENEMY = 'x', PLAYER = '+'
	};

	struct Dimensions {
		u16 height, width;
	};

	struct Position {
		u16 y, x;

		friend bool operator==(const Position& lhs, const Position& rhs) = default;
		Position& operator+=(const Dimensions& rhs);
		Position& operator%=(const Dimensions& rhs);
		friend std::ostream& operator<<(std::ostream& stream, const Position& position);
	};

	class Map {
	public:
		Map(const std::string& fileName);

		Dimensions getDimensions() const;
		Field getField(Position position) const;
		void setField(Position position, Field newField);

	private:
		u16 rowSize;
		std::vector<Field> fields;
	};
}