#ifndef __UTILS_H
#define __UTILS_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define CHK(op)       \
  do {                \
    if ((op) == -1) { \
      raler(1, #op);  \
    }                 \
  } while (0)

noreturn void raler(int syserr, const char *signal, ...);

const char *extension_fichier(const char *fichier);

unsigned long hex2dec(char *hex);
char *dec2hex(unsigned decimal, char *hex, unsigned size, bool fill);

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