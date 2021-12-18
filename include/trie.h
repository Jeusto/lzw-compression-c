#ifndef __TRIE_H
#define __TRIE_H

#include <stdbool.h>
#include <stdio.h>

#define NOMBRE_FILS 256

typedef struct trie_noeud {
  bool est_cle_valide;
  struct trie_noeud *fils[NOMBRE_FILS];
  char *valeur;

} * TrieNoeud;

TrieNoeud creer_noeud();
void liberer_trie(TrieNoeud trie);
void inserer_trie(TrieNoeud trie, const *str);
bool recherche_trie(TrieNoeud trie, const *str);

#endif