#ifndef GAME_HPP
#define GAME_HPP

#include "misc.hpp"
#include "map.hpp"
#include <vector>
#include <string>
#include <olcPixelGameEngine.hpp>
#include <ostream>

namespace Pacman {
	class Game : private olc::PixelGameEngine {
	public:
		struct Entity {
			enum class Direction : u8 {
				STOP = 0, LEFT, RIGHT, UP, DOWN
			};

			Map::Position position;
			Direction direction;
			Map::Field field;

			friend std::ostream& operator<<(std::ostream& stream, const Entity& entity);
		};

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

	public:
		Game(const std::string& map_file_name, u16 ability_duration, float fps, u16 update_frequency);

		void start();
	};
}

#endif // GAME_HPP