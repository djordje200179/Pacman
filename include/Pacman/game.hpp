#pragma once

#include "misc.hpp"
#include "map.hpp"
#include "entity.hpp"
#include <vector>
#include <string>
#include <olcPixelGameEngine.hpp>
#include <ostream>

namespace Pacman {
	class Game : private olc::PixelGameEngine {
	public:
		Game(const std::string& map_file_name, u16 ability_duration, float fps, u16 update_frequency);

		void start();

	private:
		const float target_frame_time;
		const u16 ability_duration;
		const u16 update_frequency;

		Map map;
		Entity player;
		std::vector<Entity> enemies;

		u16 score = 0;
		u16 ability_counter = 0;
		bool is_running = false;

		Position generate_new_position(Position old_position, Direction direction);
		void move_entity(Entity& entity);
		void move_player();
		void move_enemies();

		void set_initial_positions();

		void draw();
		void update();
		void handle_events();
		void log() const;

		bool OnUserCreate() override;
		bool OnUserUpdate(float elapsed_time) override;
	};
}