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
		Map(const std::string& file_name);

		Dimensions get_dimensions() const;
		Field get_field(Position position) const;
		void set_field(Position position, Field new_field);

	private:
		u16 row_size;
		std::vector<Field> fields;
	};
}