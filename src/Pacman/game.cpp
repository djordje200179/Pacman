#include "Pacman/game.hpp"
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

namespace Pacman {
	Game::Game(const std::string& map_file_name, u16 ability_duration) : ability_duration(ability_duration), map(map_file_name) {
		setup_console();
		set_initial_positions();
	}

	void Game::move_entity(Entity& entity) {
		auto old_position = entity.position;
		auto new_position = old_position;

		switch(entity.direction) {
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

		auto dimensions = map.get_dimensions();

		new_position.x = (new_position.x + dimensions.width) % dimensions.width;
		new_position.y = (new_position.y + dimensions.height) % dimensions.height;

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
					enemies.emplace_back(position, Entity::Direction::UP, Map::Field::SPACE);
					break;
				case Map::Field::PLAYER:
					player = { position, Entity::Direction::STOP, Map::Field::SPACE };
					break;
				}
			}
	}

	void Game::print() const {
		reset_cursor();

		std::cout << CYAN_COLOR;
		std::cout << "Score: " << score << '\t';
		if(ability_counter)
			std::cout << "Ability: " << ability_counter << "  " << '\t';
		else
			std::cout << "                 ";
		std::cout << NO_COLOR << '\n';

		map.print();
	}

	void Game::update() {
		move_player();
		move_enemies();

		if(ability_counter > 0)
			ability_counter--;
	}

	void Game::event_handler() {
		switch(get_pressed_key()) {
		case Key::ESCAPE:
			is_running = false;
			break;
		case Key::ARROW_UP:
			player.direction = Entity::Direction::UP;
			break;
		case Key::ARROW_DOWN:
			player.direction = Entity::Direction::DOWN;
			break;
		case Key::ARROW_LEFT:
			player.direction = Entity::Direction::LEFT;
			break;
		case Key::ARROW_RIGHT:
			player.direction = Entity::Direction::RIGHT;
			break;
		}
	}

	void Game::loop(u16 fps, u16 move_ratio) {
		is_running = true;
		while(is_running) {
			static u16 time_counter = 0;
			if(time_counter == fps / move_ratio) {
				update();
				print();

				time_counter = 0;
			}
			time_counter++;

			event_handler();

			using namespace std::chrono_literals;
			std::this_thread::sleep_for(1000ms / fps);
		}
	}
}