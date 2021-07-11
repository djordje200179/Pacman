#include "game.hpp"
#include "misc.hpp"
#include <Windows.h>
#include <conio.h>

#define FPS 60
#define MOVE_RATIO 15

int main(int argc, char** argv) {
	setup_console();

	auto game = Game("field.maka", 60);
	auto game_running = true;
	while(game_running) {
		static u8 time_counter = 0;
		if(time_counter == FPS / MOVE_RATIO) {
			game.update();
			game.print();

			time_counter = 0;
		}
		time_counter++;

		if(_kbhit()) {
			auto pressed = (char)_getch();
			switch(pressed) {
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