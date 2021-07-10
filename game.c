#include "game.h"
#include <stdlib.h>
#include <stdio.h>

Game* init_game(char map_file[]) {
	Game* game = (Game*)malloc(sizeof(Game));

	game->map = load_map(map_file);

	game->enemies = NULL;
	game->enemies_count = 0;

	for(u8 i = 0; i < game->map->height; i++)
		for(u8 j = 0; j < game->map->width; j++) {
			if(game->map->fields[i][j] == ENEMY) {
				game->enemies_count++;
				game->enemies = (Entity*)realloc(game->enemies, sizeof(Entity) * game->enemies_count);

				game->enemies[game->enemies_count - 1].position = (Pair) { j, i };
				game->enemies[game->enemies_count - 1].direction = STOP;
			} else if(game->map->fields[i][j] == PLAYER) {
				game->player.position = (Pair) { j, i };
				game->player.direction = STOP;
			}
		}

	game->score = 0;
	game->ability_count = 0;

	return game;
}

void end_game(Game* game) {
	destroy_map(game->map);
	free(game);
}

void print_state(Game* game) {
	reset_cursor();

	printf(CYAN_COLOR);
	printf("Score: %hu\t", game->score);
	if(game->ability_count)
		printf("Ability: %hhu\t", game->ability_count);
	else
		printf("               ");
	printf("\n");
	printf(NO_COLOR);

	print_map(game->map);
}

static void move_player(Game* game) {
	Pair old_position = game->player.position;
	Pair new_position = old_position;
	switch(game->player.direction) {
	case UP:
		new_position.y--;
		break;
	case DOWN:
		new_position.y++;
		break;
	case LEFT:
		new_position.x--;
		break;
	case RIGHT:
		new_position.x++;
		break;
	}

	new_position.x = (new_position.x + game->map->width) % game->map->width;
	new_position.y = (new_position.y + game->map->height) % game->map->height;

	Field* old_field = &game->map->fields[old_position.y][old_position.x];
	Field* new_field = &game->map->fields[new_position.y][new_position.x];

	switch(*new_field) {
	case WALL:
		game->player.direction = STOP;

		break;
	case ABILITY:
		game->ability_count = ABILITY_LENGTH;
	case FOOD:
		game->score++;
	case SPACE:
		*old_field = SPACE;
		*new_field = PLAYER;
		game->player.position = new_position;

		break;
	}
}

static void decrease_ability(Game* game) {
	if(game->ability_count > 0)
		game->ability_count--;
}

void update_state(Game* game) {
	move_player(game);
	decrease_ability(game);
}