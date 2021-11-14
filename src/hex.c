// More info about the functions here:
// https://nachtimwald.com/2017/09/24/hex-encode-and-decode-in-c/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned long hex2dec(char *hex) { return strtoul(hex, NULL, 16); }

// inspired from https://www.sanfoundry.com/c-program-convert-decimal-hex/
char *dec2hex(unsigned long decimal, char *hex, unsigned size, bool fill) {
  char *tmp = (char *)malloc((size) * sizeof(char));

  if (hex == NULL) {
    // size + 1 to take into account the '\0' string code
    hex = (char *)malloc((size + 1) * sizeof(char));
    hex[size] = '\0';
  }

  long quotient, remainder;
  int hex_size = 0;
  quotient = decimal;

  while (quotient != 0) {
    remainder = quotient % 16;
    if (remainder < 10) {
      tmp[hex_size] = 48 + remainder;
    } else {
      tmp[hex_size] = 55 + remainder;
    }
    hex_size++;
    quotient = quotient / 16;
  }

  // nb of zero to output
  int zero_left = size - hex_size;

  // write hex value to the end
  while (hex_size >= 0) {
    hex[size - hex_size - 1] = tmp[hex_size];
    hex_size--;
  }

  // complete hex with 0
  while (zero_left > 0) {
    hex[zero_left - 1] = '0';
    zero_left--;
  }

  free(tmp);

  return hex;
}