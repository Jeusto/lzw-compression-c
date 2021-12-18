#include <argp.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "../include/hashmap.h"
#include "../include/liste-chainee.h"
#include "../include/trie.h"
#include "../include/utils.h"

/**
 * @brief Gere l'execution du programme
 *
 * @param argc : Le nombre d'arguments de la ligne de commande
 * @param argv : Les arguments
 * @return int : 0 si le programme a reussi
 */
int main(int argc, char *argv[]);