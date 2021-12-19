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

/*
 * @brief Crée un nouveau noeud de trie.
 *
 * @return le nouveau noeud
 */
TrieNoeud creer_noeud_trie();

/*
 * @brief Insère dans la trie un couple clé/valeur.
 *
 * @param liste: la trie à parcourir
 * @param clé: la cle qui permet de rechercher un couple clé/valeur
 * @param valeur: la valeur associée à la clé
 *
 * @return la trie avec le nouveau couple clé/valeur
 */
TrieNoeud inserer_trie(TrieNoeud trie, char *cle, char *valeur);

/*
 * @brief Recherche dans la trie une clé et renvoie la valeur
 * associée.
 *
 * @param trie: la trie à parcourir
 * @param clé: la clé à rechercher
 *
 * @return la valeur associée à la clé
 */
char *recuperer_trie(TrieNoeud trie, char *cle);

/*
 * @brief Libére l'emplacement mémoire associée à une trie.
 *
 * @param trie: la trie à libérer
 */
void liberer_trie(TrieNoeud trie);

#endif