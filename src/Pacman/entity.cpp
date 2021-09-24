#include "Pacman/entity.hpp"

namespace Pacman {
	std::ostream& operator<<(std::ostream& stream, const Direction& direction) {
		switch(direction) {
		case Direction::STOP:
			stream << "STOP";
			break;
		case Direction::LEFT:
			stream << "LEFT";
			break;
		case Direction::RIGHT:
			stream << "RIGHT";
			break;
		case Direction::UP:
			stream << "UP";
			break;
		case Direction::DOWN:
			stream << "DOWN";
			break;
		}

		return stream;
	}

	std::ostream& operator<<(std::ostream& stream, const Entity& entity) {
		stream << entity.position << ' ' << entity.direction;
		return stream;
	}
}