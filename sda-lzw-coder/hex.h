#ifndef __HEX_H
#define __HEX_H

#include <stdlib.h>

unsigned long hex2dec(char* hex);
char *dec2hex(unsigned decimal, char* hex, unsigned size, bool fill);

#endif