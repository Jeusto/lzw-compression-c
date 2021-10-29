#ifndef __TRIE_H
#define __TRIE_H

#include <stdbool.h>
#include <stdio.h>

#define NOMBRE_FILS 256

typedef struct Noeud_Trie {
  bool est_cle_valide;
  struct Noeud_Trie *fils[NOMBRE_FILS];
} Noeud_Trie;

Noeud_Trie *creer_noeud();
void liberer_trie(Noeud_Trie *trie);
void insertion_trie(Noeud_Trie *trie, const char *str);
bool recherche_trie(Noeud_Trie *trie, const char *str);

#endif