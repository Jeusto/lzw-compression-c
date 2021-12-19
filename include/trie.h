#ifndef __TRIE_H
#define __TRIE_H

#include <stdbool.h>
#include <stdio.h>

#define NOMBRE_FILS 256

typedef struct trie_noeud {
  bool est_cle_valide;
  char *valeur;
  struct trie_noeud *fils[NOMBRE_FILS];
} * TrieNoeud;

TrieNoeud creer_noeud_trie();
void liberer_trie(TrieNoeud trie);
TrieNoeud inserer_trie(TrieNoeud trie, char *cle, char *valeur);
char *recuperer_trie(TrieNoeud trie, char *cle);

#endif