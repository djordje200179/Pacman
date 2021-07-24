#include "Pacman/game.hpp"
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

namespace Pacman {
	std::ostream& operator<<(std::ostream& stream, const Game::Entity::Direction& direction) {
		using enum Game::Entity::Direction;
		switch(direction) {
		case STOP:
			stream << "STOP";
			break;
		case LEFT:
			stream << "LEFT";
			break;
		case RIGHT:
			stream << "RIGHT";
			break;
		case UP:
			stream << "UP";
			break;
		case DOWN:
			stream << "DOWN";
			break;
		}
		
		return stream;
	}

	std::ostream& operator<<(std::ostream& stream, const Game::Entity& entity) {
		stream << entity.position << ' ' << entity.direction;
		return stream;
	}

	Game::Game(const std::string& map_file_name, u16 ability_duration, float fps, u16 update_frequency) :
		ability_duration(ability_duration), map(map_file_name), target_frame_time(1.0f / fps), update_frequency(update_frequency) {
		sAppName = "Pacman";
	}

	Map::Position Game::generate_new_position(Map::Position old_position, Entity::Direction direction) {
		auto dimensions = map.get_dimensions();
		
		auto new_position = old_position;
		new_position += dimensions;

		switch(direction) {
		case Entity::Direction::UP:
			new_position.y--;
			break;
		case Entity::Direction::DOWN:
			new_position.y++;
			break;
		case Entity::Direction::LEFT:
			new_position.x--;
			break;
		case Entity::Direction::RIGHT:
			new_position.x++;
			break;
		}

		new_position %= dimensions;

		return new_position;
	}

	void Game::move_entity(Entity& entity) {
		if(entity.direction == Entity::Direction::STOP)
			return;

		auto old_position = entity.position;
		auto new_position = generate_new_position(old_position, entity.direction);

		auto& underlying_field = entity.field;
		auto& old_field = map.get_field(old_position);
		auto& new_field = map.get_field(new_position);

		if(new_field == Map::Field::WALL)
			entity.direction = Entity::Direction::STOP;

		if(entity.direction != Entity::Direction::STOP) {
			entity.position = new_position;

			Map::Field old_field_copy(old_field);
			old_field = Map::Field(underlying_field);
			underlying_field = Map::Field(new_field);
			new_field = old_field_copy;
		}
	}

	void Game::move_player() {
		move_entity(player);

		switch(player.field) {
		case Map::Field::ENEMY:
			if(ability_counter) {
				enemies.erase(std::find_if(enemies.begin(), enemies.end(),
							  [&](const Entity& enemy) { return enemy.position == player.position; })
				);

				player.field = Map::Field::SPACE;
			} else {
				is_running = false;

				map.get_field(player.position) = Map::Field::ENEMY;
			}

			break;
		case Map::Field::ABILITY:
			ability_counter = ability_duration;
		case Map::Field::FOOD:
			score++;
			player.field = Map::Field::SPACE;

			break;
		}
	}

	void Game::move_enemies() {
		for(auto& enemy : enemies) {
			move_entity(enemy);

			if(enemy.field == Map::Field::PLAYER)
				is_running = false;
		}
	}

	void Game::set_initial_positions() {
		auto dimensions = map.get_dimensions();

		for(u16 i = 0; i < dimensions.height; i++)
			for(u16 j = 0; j < dimensions.width; j++) {
				Map::Position position { i, j };

				switch(map.get_field(position)) {
				case Map::Field::ENEMY:
					enemies.emplace_back(position, Entity::Direction::STOP, Map::Field::SPACE);
					break;
				case Map::Field::PLAYER:
					player = { position, Entity::Direction::STOP, Map::Field::SPACE };
					break;
				}
			}
	}

	void Game::draw() {
		auto dimensions = map.get_dimensions();

		for(u16 i = 0; i < dimensions.height; i++)
			for(u16 j = 0; j < dimensions.width; j++) {
				Map::Position position { i, j };
				olc::Pixel pixel_color = olc::WHITE;
				Map::Field& field = map.get_field(position);

				switch(field) {
				case Map::Field::SPACE:
					pixel_color = olc::BLACK;
					break;
				case Map::Field::WALL:
					pixel_color = olc::WHITE;
					break;
				case Map::Field::FOOD:
					pixel_color = olc::YELLOW;
					break;
				case Map::Field::ABILITY:
					pixel_color = olc::DARK_YELLOW;
					break;
				case Map::Field::ENEMY:
					pixel_color = olc::RED;
					break;
				case Map::Field::PLAYER:
					pixel_color = olc::DARK_GREEN;
					break;
				}

				switch(field) {
				case Map::Field::FOOD:
					Draw(j * 5 + 2, i * 5 + 2, pixel_color);

					break;
				case Map::Field::SPACE:
				case Map::Field::WALL:
					for(u8 k = 0; k < 5; k++)
						for(u8 l = 0; l < 5; l++)
							Draw(j * 5 + l, i * 5 + k, pixel_color);

					break;
				case Map::Field::ABILITY:
				case Map::Field::ENEMY:
				case Map::Field::PLAYER:
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
			player.direction = Entity::Direction::UP;

		if(GetKey(olc::Key::DOWN).bPressed)
			player.direction = Entity::Direction::DOWN;

		if(GetKey(olc::Key::LEFT).bPressed)
			player.direction = Entity::Direction::LEFT;

		if(GetKey(olc::Key::RIGHT).bPressed)
			player.direction = Entity::Direction::RIGHT;
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

			return true;
		}
		else
			return true;
	}

	void Game::start() {
		Construct(240, 160, 4, 4);
		Start();
	}
}