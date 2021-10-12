#pragma once

#include "misc.hpp"
#include "map.hpp"

namespace Pacman {
enum class Direction : u8 {
	STOP = 0, LEFT, RIGHT, UP, DOWN
};

struct Entity {
	Position position;
	Direction direction;
	Field field;

	friend std::ostream& operator<<(std::ostream& stream, const Entity& entity);
};
}