#ifndef MAP
#define MAP

#include "misc.hpp"
#include <string>
#include <vector>

class Map {
public:
	struct Position {
		u8 y, x;
	};

	struct Dimensions {
		u8 height, width;
	};

	enum class Field : char {
		SPACE = ' ', WALL = '#', FOOD = '.', ABILITY = 'O', ENEMY = 'x', PLAYER = '+'
	};
private:
	std::vector<std::vector<Field>> fields;

public:
	Map(const std::string& file_name);

	void print() const;

	Dimensions get_dimensions() const;
	Field& get_field(Position position);
};

#endif // MAP