#ifndef MAP
#define MAP

#include "misc.h"

typedef enum Field {
	SPACE = ' ', WALL = '#', FOOD = '.', ABILITY = 'O', ENEMY = 'x', PLAYER = '+'
} Field;

typedef struct Map {
	Field** fields;

	u8 width, height;
} Map;

Map* load_map(char file_name[]);
void destroy_map(Map* map);

void print_map(Map* map);

#endif // MAP