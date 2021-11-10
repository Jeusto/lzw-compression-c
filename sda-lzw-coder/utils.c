#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "utils.h"
#include "hex.h"

/**
 * Remove char from string at the given index
 */
void remove_char(char *word, size_t index) {
    memmove(&word[index],&word[index+1],strlen(word)-index);
}

/**
 * Remove all char from given string that aren't a-z 
 */
void parse_word(char *word) {
    for(size_t i = 0; i < strlen(word); i++) {
        size_t index = (size_t) word[i];
        if(index < 97 || index > 122) {
            remove_char(word, i);
        }
    }
}

/**
 * Concat two strings together
 */
char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in production-ready code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

char* char2str(const char c) {
    char *str = malloc(2 * sizeof(char));
    str[0] = c;
    str[1] = '\0';

    return str;
}

/**
 *  Convert hex string to short (most of the time 2 bytes on modern system)
 *  and write it in a file 
 */
bool wb_hex_as_short(FILE *fp, char* hex) {
    unsigned short bin = (unsigned short) hex2dec(hex);
    // true if fwrite wrote one elt, false otherwise
    return fwrite(&bin, sizeof(unsigned int), 1, fp) == 1;
}

/**
 *  Convert hex string to int (most of the time 4 bytes on modern system)
 *  and write it in a file 
 */
bool wb_hex_as_int(FILE *fp, char* hex) {
    unsigned int bin = (unsigned int) hex2dec(hex);
    // true if fwrite wrote one elt, false otherwise
    return fwrite(&bin, sizeof(unsigned int), 1, fp) == 1;
}

/**
 *  Convert hex string to long (most of the time 8 bytes on modern system)
 *  and write it in a file 
 */
bool wb_hex_as_long(FILE *fp, char* hex) {
    unsigned long bin = (unsigned long) hex2dec(hex);
    // true if fwrite wrote one elt, false otherwise
    return fwrite(&bin, sizeof(unsigned long), 1, fp) == 1;
}

/**
 * Read next bytes of file stream as a short (most of the time 2 bytes on modern system)
 * and write it in a file
 */
char* rb_next_short_as_hex(FILE *fp) {
    unsigned short input;
    if(fread(&input, sizeof(unsigned short), 1, fp) == 0) {
        return NULL;
    }
    else {
        return dec2hex(input, NULL, 8, false);
    }
}

/**
 * Read next bytes of file stream as a int (most of the time 4 bytes on modern system)
 * and write it in a file
 */
char* rb_next_int_as_hex(FILE *fp) {
    unsigned int input;
    if(fread(&input, sizeof(unsigned int), 1, fp) == 0) {
        return NULL;
    }
    else {
        return dec2hex(input, NULL, 8, false);
    }
}

/**
 * Read next bytes of file stream as a long (most of the time 8 bytes on modern system)
 * and write it in a file
 */
char* rb_next_long_as_hex(FILE *fp) {
    unsigned long input;
    if(fread(&input, sizeof(unsigned long), 1, fp) == 0) {
        return NULL;
    }
    else {
        return dec2hex(input, NULL, 8, false);
    }
}