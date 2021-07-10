#include "game.h"
#include "misc.h"
#include <stdbool.h>
#include <Windows.h>
#include <conio.h>

#define FPS 60
#define MOVE_RATIO 10

int main(int argc, char** argv) {
	setup_console();

	Game* game = init_game("field.maka");

	bool game_running = true;
	while(game_running) {
		static u8 time_counter = 0;

		print_state(game);

		if(_kbhit()) {
			switch(getch()) {
			case ESCAPE:
				game_running = false;
				break;
			case ARROW_UP:
				game->player.direction = UP;
				break;
			case ARROW_DOWN:
				game->player.direction = DOWN;
				break;
			case ARROW_LEFT:
				game->player.direction = LEFT;
				break;
			case ARROW_RIGHT:
				game->player.direction = RIGHT;
				break;
			}
		}

		if(time_counter == FPS / MOVE_RATIO) {
			update_state(game);

			time_counter = 0;
		}

		time_counter++;

		Sleep(1000 / FPS);
	}

	end_game(game);

	return 0;
}