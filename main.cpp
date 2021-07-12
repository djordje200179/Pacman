#include "game.hpp"
#include "misc.hpp"
#include <Windows.h>
#include <conio.h>

const std::string MAP = "field.maka";
const u16 FPS = 60;
const u16 MOVE_RATIO = 15;
const u16 ABILITY_DURATION = 60;

int main(int argc, char** argv) {
	setup_console();

	auto game = Game(MAP, ABILITY_DURATION);
	auto game_running = true;
	while(game_running) {
		static u16 time_counter = 0;
		if(time_counter == FPS / MOVE_RATIO) {
			game.update();
			game.print();

			time_counter = 0;
		}
		time_counter++;

		if(_kbhit()) {
			auto pressed_key = static_cast<char>(_getch());
			switch(pressed_key) {
			case ESCAPE:
				game_running = false;
				break;
			case ARROW_UP:
				game.change_player_direction(Game::Entity::Direction::UP);
				break;
			case ARROW_DOWN:
				game.change_player_direction(Game::Entity::Direction::DOWN);
				break;
			case ARROW_LEFT:
				game.change_player_direction(Game::Entity::Direction::LEFT);
				break;
			case ARROW_RIGHT:
				game.change_player_direction(Game::Entity::Direction::RIGHT);
				break;
			}
		}

		Sleep(1000 / FPS);
	}

	return 0;
}