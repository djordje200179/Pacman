#include "game.hpp"
#include <iostream>

Game::Game(std::string map_file_name, u8 ability_duration) : ability_duration(ability_duration), map(map_file_name) {
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

	for(u8 i = 0; i < dimensions.height; i++)
		for(u8 j = 0; j < dimensions.width; j++) {
			auto position = Map::Position { i, j };
			switch(map.get_field(position)) {
			case Map::Field::ENEMY:
				enemies.push_back({ position, Entity::Direction::STOP });
				break;
			case Map::Field::PLAYER:
				player = { position, Entity::Direction::STOP };
				break;
			}
		}
}

void Game::print() const {
	reset_cursor();

	std::cout << CYAN_COLOR;
	std::cout << "Score: " << score << '\t';
	if(ability_counter)
		std::cout << "Ability: " << (u16)ability_counter << '\t';
	std::cout << "                 " << '\n' << NO_COLOR;

	map.print();
}

void Game::update() {
	move_player();

	if(ability_counter > 0)
		ability_counter--;
}

void Game::change_player_direction(Entity::Direction direction) {
	player.direction = direction;
}