#ifndef __TRIE_H
#define __TRIE_H

#include <stdbool.h>
#include <stdio.h>

#define NOMBRE_FILS 256

typedef struct TrieNoeud {
  bool est_cle_valide;
  struct TrieNoeud *fils[NOMBRE_FILS];
} TrieNoeud;

TrieNoeud *creer_noeud();
void liberer_trie(TrieNoeud *trie);
void inserer_trie(TrieNoeud *trie, const char *str);
bool recherche_trie(TrieNoeud *trie, const char *str);

#endif