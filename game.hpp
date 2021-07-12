#ifndef GAME_HPP
#define GAME_HPP

#include "misc.hpp"
#include "map.hpp"
#include <vector>

class Game {
public:
	struct Entity {
		enum class Direction : u8 {
			STOP = 0, LEFT, RIGHT, UP, DOWN
		};

		Map::Position position;
		Direction direction;
	};
private:
	Map map;

	u16 score = 0;
	u16 ability_counter = 0;
	u16 ability_duration;

	Entity player;
	std::vector<Entity> enemies;

	void move_player();
	void set_initial_positions();

public:
	Game(const std::string& map_file_name, u16 ability_duration);

	void print() const;
	void update();
	void change_player_direction(Entity::Direction direction);
};

#endif // GAME_HPP