#include "Pacman/game.hpp"
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

namespace Pacman {
	Game::Game(const std::string& map_file_name, u16 ability_duration, float fps, u16 update_frequency) :
		ability_duration(ability_duration), map(map_file_name), target_frame_time(1.0f / fps), update_frequency(update_frequency) {
		sAppName = "Pacman";
	}

	Position Game::generate_new_position(Position old_position, Direction direction) {
		auto dimensions = map.get_dimensions();
		
		auto new_position = old_position;
		new_position += dimensions;

		switch(direction) {
		case Direction::UP:
			new_position.y--;
			break;
		case Direction::DOWN:
			new_position.y++;
			break;
		case Direction::LEFT:
			new_position.x--;
			break;
		case Direction::RIGHT:
			new_position.x++;
			break;
		}

		new_position %= dimensions;

		return new_position;
	}

	void Game::move_entity(Entity& entity) {
		if(entity.direction == Direction::STOP)
			return;

		auto old_position = entity.position;
		auto new_position = generate_new_position(old_position, entity.direction);

		auto& underlying_field = entity.field;
		auto& old_field = map.get_field(old_position);
		auto& new_field = map.get_field(new_position);

		if(new_field == Field::WALL)
			entity.direction = Direction::STOP;

		if(entity.direction != Direction::STOP) {
			entity.position = new_position;

			Field old_field_copy(old_field);
			old_field = Field(underlying_field);
			underlying_field = Field(new_field);
			new_field = old_field_copy;
		}
	}

	void Game::move_player() {
		move_entity(player);

		switch(player.field) {
		case Field::ENEMY:
			if(ability_counter) {
				enemies.erase(std::find_if(enemies.begin(), enemies.end(),
							  [&](const Entity& enemy) { return enemy.position == player.position; })
				);

				player.field = Field::SPACE;
			} else {
				is_running = false;

				map.get_field(player.position) = Field::ENEMY;
			}

			break;
		case Field::ABILITY:
			ability_counter = ability_duration;
		case Field::FOOD:
			score++;
			player.field = Field::SPACE;

			break;
		}
	}

	void Game::move_enemies() {
		for(auto& enemy : enemies) {
			move_entity(enemy);

			if(enemy.field == Field::PLAYER)
				is_running = false;
		}
	}

	void Game::set_initial_positions() {
		auto dimensions = map.get_dimensions();

		for(u16 i = 0; i < dimensions.height; i++)
			for(u16 j = 0; j < dimensions.width; j++) {
				Position position { i, j };

				switch(map.get_field(position)) {
				case Field::ENEMY:
					enemies.emplace_back(position, Direction::STOP, Field::SPACE);
					break;
				case Field::PLAYER:
					player = { position, Direction::STOP, Field::SPACE };
					break;
				}
			}
	}

	void Game::draw() {
		auto dimensions = map.get_dimensions();

		for(u16 i = 0; i < dimensions.height; i++)
			for(u16 j = 0; j < dimensions.width; j++) {
				Position position { i, j };
				olc::Pixel pixel_color = olc::WHITE;
				Field& field = map.get_field(position);

				switch(field) {
				case Field::SPACE:
					pixel_color = olc::BLACK;
					break;
				case Field::WALL:
					pixel_color = olc::WHITE;
					break;
				case Field::FOOD:
					pixel_color = olc::YELLOW;
					break;
				case Field::ABILITY:
					pixel_color = olc::DARK_YELLOW;
					break;
				case Field::ENEMY:
					pixel_color = ability_counter ? olc::BLUE : olc::RED;
					break;
				case Field::PLAYER:
					pixel_color = olc::DARK_GREEN;
					break;
				}

				switch(field) {
				case Field::FOOD:
					Draw(j * 5 + 2, i * 5 + 2, pixel_color);

					break;
				case Field::SPACE:
				case Field::WALL:
					for(u8 k = 0; k < 5; k++)
						for(u8 l = 0; l < 5; l++)
							Draw(j * 5 + l, i * 5 + k, pixel_color);

					break;
				case Field::ABILITY:
				case Field::ENEMY:
				case Field::PLAYER:
					for(u8 k = 0; k < 3; k++)
						for(u8 l = 0; l < 3; l++)
							Draw(j * 5 + 1 + l, i * 5 + 1 + k, pixel_color);

					break;
				}
			}
	}

	void Game::update() {
		static u16 time_counter = 0;
		if(time_counter == update_frequency) {
			move_player();
			move_enemies();

			if(ability_counter > 0)
				ability_counter--;

			time_counter = 0;
		}

		time_counter++;
	}

	void Game::handle_events() {
		if(GetKey(olc::Key::ESCAPE).bPressed)
			is_running = false;

		if(GetKey(olc::Key::UP).bPressed)
			player.direction = Direction::UP;

		if(GetKey(olc::Key::DOWN).bPressed)
			player.direction = Direction::DOWN;

		if(GetKey(olc::Key::LEFT).bPressed)
			player.direction = Direction::LEFT;

		if(GetKey(olc::Key::RIGHT).bPressed)
			player.direction = Direction::RIGHT;
	}

	void Game::log() const {
		std::cout << player << std::endl;
	}

	bool Game::OnUserCreate() {
		set_initial_positions();
		is_running = true;

		return true;
	}

	bool Game::OnUserUpdate(float elapsed_time) {
		if(!is_running)
			return false;

		handle_events();

		static float accumulated_time = 0.0f;

		accumulated_time += elapsed_time;
		if(accumulated_time >= target_frame_time) {
			accumulated_time -= target_frame_time;

			log();
			update();
			draw();
		}

		return true;
	}

	void Game::start() {
		Construct(240, 160, 4, 4);
		Start();
	}
}