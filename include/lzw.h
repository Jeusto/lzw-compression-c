
#ifndef __LZW_H
#define __LZW_H

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
#include "../include/liste.h"
#include "../include/trie.h"
#include "../include/utils.h"

#define TAILLE_DICT 65536
#define TAILLE_MAX_HEXA_STRING 9
#define TAILLE_MAX_STRING 64

ListeNoeud initialiser_liste(char* mode);

TrieNoeud initialiser_trie(char* mode);

struct hashmap_s initialiser_hashmap(char* mode, char** tableau_cles,
                                     char** tableau_valeurs);

void compresser_trie(TrieNoeud dict, FILE* fichier_source,
                     FILE* fichier_destination);

void decompresser_trie(TrieNoeud dict, FILE* fichier_source,
                       FILE* fichier_destination);

void liberer_dictionnaire_hashmap(struct hashmap_s* dict, char** tableau_cles,
                                  char** tableau_valeurs);

void compresser_hashmap(FILE* fichier_source, FILE* fichier_destination,
                        struct hashmap_s dict, char** tableau_cles,
                        char** tableau_valeurs);

void decompresser_hashmap(FILE* fichier_source, FILE* fichier_destination,
                          struct hashmap_s dict, char** tableau_cles,
                          char** tableau_valeurs);

/*
 * @brief conpresse un fichier en utilisant un dictionnare implémemté à partir
 * d'une liste
 *
 * @param listNoeud: la liste à initialiser
 * @param File: Le fichier à compresser
 * @param File: Le fichier compressé
 *
 */
void compresser_liste(ListeNoeud dict, FILE* fichier_source,
                      FILE* fichier_destination);

/*
 * @brief compresse un fichier en utilisant un dictionnare implémemté à partir
 * d'une liste
 *
 * @param listNoeud: la liste à initialiser
 * @param File: Le fichier à compresser
 * @param File: Le fichier compressé
 *
 */
void decompresser_liste(ListeNoeud dict, FILE* fichier_source,
                        FILE* fichier_destination);
#endif