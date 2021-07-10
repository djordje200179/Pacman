#ifndef GAME
#define GAME

#include "map.h"
#include "misc.h"

#define ABILITY_LENGTH 60

typedef struct Pair {
	u8 x, y;
} Pair;

typedef enum Direction {
	STOP = 0, LEFT, RIGHT, UP, DOWN
} Direction;

typedef struct Entity {
	Pair position;
	Direction direction;
} Entity;

typedef struct Game {
	Map* map;

	unsigned int score;
	u8 ability_count;
	Entity player;

	u8 enemies_count;
	Entity* enemies;
} Game;

Game* init_game(char map_file[]);
void end_game(Game* game);

void print_state(Game* game);
void update_state(Game* game);

#endif // GAME