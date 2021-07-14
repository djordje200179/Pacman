#include "game.hpp"
#include <iostream>
#include <Windows.h>
#include <conio.h>

Game::Game(const std::string& map_file_name, u16 ability_duration) : ability_duration(ability_duration), map(map_file_name) {
	set_initial_positions();
}

void Game::move_player() {
	auto old_position = player.position;
	auto new_position = old_position;

	switch(player.direction) {
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

	switch(new_field) {
	case Map::Field::WALL:
		player.direction = Entity::Direction::STOP;

		break;
	case Map::Field::ABILITY:
		ability_counter = ability_duration;
	case Map::Field::FOOD:
		score++;
	case Map::Field::SPACE:
		old_field = Map::Field::SPACE;
		new_field = Map::Field::PLAYER;

		player.position = new_position;

		break;
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

	if(ability_counter > 0)
		ability_counter--;
}

void Game::event_handler() {
	if(_kbhit()) {
		auto pressed_key = static_cast<char>(_getch());
		switch(pressed_key) {
		case ESCAPE:
			is_running = false;
			break;
		case ARROW_UP:
			player.direction = Entity::Direction::UP;
			break;
		case ARROW_DOWN:
			player.direction = Entity::Direction::DOWN;
			break;
		case ARROW_LEFT:
			player.direction = Entity::Direction::LEFT;
			break;
		case ARROW_RIGHT:
			player.direction = Entity::Direction::RIGHT;
			break;
		}
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

		Sleep(1000 / fps);
	}
}