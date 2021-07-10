#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "str_builder.h"

static const size_t str_builder_min_size = 32;

str_builder* str_builder_create(void) {
    str_builder* sb;

    sb = calloc(1, sizeof(*sb));
    sb->str = malloc(str_builder_min_size);
    *sb->str = '\0';
    sb->alloced = str_builder_min_size;
    sb->len = 0;

    return sb;
}

void str_builder_destroy(str_builder* sb) {
    if(sb == NULL)
        return;

    free(sb->str);
    free(sb);
}

static void str_builder_ensure_space(str_builder* sb, size_t add_len) {
    if(sb == NULL || add_len == 0)
        return;

    if(sb->alloced >= sb->len + add_len + 1)
        return;

    while(sb->alloced < sb->len + add_len + 1) {
        sb->alloced <<= 1;

        if(sb->alloced == 0)
            sb->alloced--;
    }

    sb->str = realloc(sb->str, sb->alloced);
}

void str_builder_add_str(str_builder* sb, const char* str, size_t len) {
    if(sb == NULL || str == NULL || *str == '\0')
        return;

    if(len == 0)
        len = strlen(str);

    str_builder_ensure_space(sb, len);
    memmove(sb->str + sb->len, str, len);
    sb->len += len;
    sb->str[sb->len] = '\0';
}

void str_builder_add_char(str_builder* sb, char c) {
    if(sb == NULL)
        return;

    str_builder_ensure_space(sb, 1);
    sb->str[sb->len] = c;
    sb->len++;
    sb->str[sb->len] = '\0';
}