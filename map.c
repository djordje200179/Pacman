#include "map.h"
#include "str_builder.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE 200

Map* load_map(char file_name[]) {
	Map* map = (Map*)malloc(sizeof(Map));
	map->height = map->width = 0;
	map->fields = NULL;

	FILE* file = fopen(file_name, "r");

	while(true) {
		static char line[MAX_LINE];
		if(!fgets(line, MAX_LINE - 1, file))
			break;
		
		u8 row_length = (u8)strlen(line);
		if(line[row_length - 1] == '\n') {
			line[row_length - 1] = '\0';
			row_length--;
		}

		if(map->width == 0)
			map->width = row_length;

		map->height++;
		map->fields = (Field**)realloc(map->fields, sizeof(Field*) * map->height);
		map->fields[map->height - 1] = (Field*)calloc(map->width, sizeof(Field));
		for(u8 i = 0; i < map->width; i++)
			map->fields[map->height - 1][i] = (Field)line[i];
		
	}

	fclose(file);

	return map;
}

void destroy_map(Map* map) {
	for(u8 i = 0; i < map->height; i++)
		free(map->fields[i]);
	free(map->fields);
	free(map);
}

void print_map(Map* map) {
	str_builder* output = str_builder_create();

	for(u8 i = 0; i < map->height; i++) {
		for(u8 j = 0; j < map->width; j++) {
			switch(map->fields[i][j]) {
			case FOOD:
			case ABILITY:
				str_builder_add_str(output, YELLOW_COLOR, 0);
				break;
			case ENEMY:
				str_builder_add_str(output, RED_COLOR, 0);
				break;
			case PLAYER:
				str_builder_add_str(output, GREEN_COLOR, 0);
				break;
			}

			str_builder_add_char(output, (char)map->fields[i][j]);
			str_builder_add_str(output, NO_COLOR, 0);
		}

		str_builder_add_char(output, '\n');
	}

	printf("%s", output->str);
	str_builder_destroy(output);
}