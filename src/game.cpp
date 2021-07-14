#include "game.hpp"
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

Game::Game(const std::string& map_file_name, u16 ability_duration) : ability_duration(ability_duration), map(map_file_name) {
	set_initial_positions();
}

Map::Field Game::move_entity(Entity& entity) {
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

	auto& old_field = map.get_field(old_position);
	auto& new_field = map.get_field(new_position);

	auto old_field_copy = old_field;
	auto new_field_copy = new_field;

	if(new_field == Map::Field::WALL)
		entity.direction = Game::Entity::Direction::STOP;

	if(entity.direction != Entity::Direction::STOP) {
		new_field = old_field_copy;
		old_field = Map::Field::SPACE;

		entity.position = new_position;
	}

	return new_field_copy;
}

void Game::move_player() {
	auto new_field_type = move_entity(player);

	switch(new_field_type) {
	case Map::Field::ENEMY:
		if(ability_counter) {
			score += 10;

			enemies.erase(std::find_if(enemies.begin(), enemies.end(), 
						  [&](const Entity& enemy) { return enemy.position == player.position; })
			);
		} else {
			map.get_field(player.position) = Map::Field::ENEMY;
			is_running = false;
		}
		break;
	case Map::Field::ABILITY:
		ability_counter = ability_duration;
		break;
	case Map::Field::FOOD:
		score++;
		break;
	}
}

void Game::move_enemies() {
	for(auto& enemy : enemies) {
		auto new_field_type = move_entity(enemy);

		if(new_field_type == Map::Field::PLAYER)
			is_running = false;
	}
}

void Game::set_initial_positions() {
	auto dimensions = map.get_dimensions();

	for(u16 i = 0; i < dimensions.height; i++)
		for(u16 j = 0; j < dimensions.width; j++) {
			auto position = Map::Position { i, j };
			switch(map.get_field(position)) {
			case Map::Field::ENEMY:
				enemies.emplace_back(position, Entity::Direction::STOP);
				break;
			case Map::Field::PLAYER:
				player = Entity(position, Entity::Direction::STOP);
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

		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / fps));
	}
}