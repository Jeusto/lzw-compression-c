#ifndef __UTILS_H
#define __UTILS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

size_t ascii_to_index(char c);
void remove_char(char *word, size_t index);
void parse_word(char *word);
char *concat(const char *s1, const char *s2);
char *char2str(const char c);
bool wb_hex_as_short(FILE *fp, char *hex);
bool wb_hex_as_int(FILE *fp, char *hex);
bool wb_hex_as_long(FILE *fp, char *hex);
char *rb_next_short_as_hex(FILE *fp);
char *rb_next_int_as_hex(FILE *fp);
char *rb_next_long_as_hex(FILE *fp);

#endif