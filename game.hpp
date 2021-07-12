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

	bool is_running = false;
	u16 score = 0;
	u16 ability_counter = 0;
	u16 ability_duration;

	Entity player;
	std::vector<Entity> enemies;

	void move_player();
	void set_initial_positions();

	void print() const;
	void update();
	void event_handler();

public:
	Game(const std::string& map_file_name, u16 ability_duration);

	void loop(u16 fps, u16 move_ratio);
};

#endif // GAME_HPP