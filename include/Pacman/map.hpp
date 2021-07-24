#pragma once

#include "misc.hpp"
#include <string>
#include <ostream>
#include <vector>

namespace Pacman {
	class Map {
	public:
		struct Dimensions {
			u16 height, width;
		};

		struct Position {
			u16 y, x;

			bool operator==(const Position& rhs) const = default;
			Position& operator+=(const Dimensions& rhs);
			Position& operator%=(const Dimensions& rhs);
			friend std::ostream& operator<<(std::ostream& stream, const Position& position);
		};

		enum class Field : char {
			SPACE = ' ', WALL = '#', FOOD = '.', ABILITY = 'O', ENEMY = 'x', PLAYER = '+'
		};

	private:
		std::vector<std::vector<Field>> fields;

	public:
		Map(const std::string& file_name);

		Dimensions get_dimensions() const;
		Field& get_field(Position position);
	};
}