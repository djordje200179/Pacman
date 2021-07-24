#ifndef MAP_HPP
#define MAP_HPP

#include "misc.hpp"
#include <string>
#include <ostream>
#include <vector>

namespace Pacman {
	class Map {
	public:
		struct Position {
			u16 y, x;

			bool operator==(const Position&) const = default;
			friend std::ostream& operator<<(std::ostream& stream, const Position& position);
		};

		struct Dimensions {
			u16 height, width;
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

#endif // MAP_HPP