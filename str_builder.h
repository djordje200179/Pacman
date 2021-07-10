#ifndef STR_BUILDER
#define STR_BUILDER

#include <stddef.h>

typedef struct str_builder {
	char* str;
	size_t alloced;
	size_t len;
} str_builder;

str_builder* str_builder_create(void);
void str_builder_destroy(str_builder* sb);

void str_builder_add_str(str_builder* sb, const char* str, size_t len);
void str_builder_add_char(str_builder* sb, char c);

#endif // STR_BUILDER