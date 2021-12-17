#include "../include/trie.h"

#include <stdlib.h>
#include <string.h>

TrieNoeud *creer_noeud() {
  // Allouer espace memoire pour le noeud
  TrieNoeud *nouveau_noeud = malloc(sizeof(struct TrieNoeud));

  if (nouveau_noeud == NULL) {
    printf("erreur");
    exit(1);
  }

  // Initialiser le noeud et ses fils
  nouveau_noeud->est_cle_valide = false;
  for (size_t i = 0; i < NOMBRE_FILS; i++) {
    nouveau_noeud->fils[i] = NULL;
  }

  return nouveau_noeud;
};

void inserer_trie(TrieNoeud *trie, const char *str) {
  // TrieNoeud n'existe pas, on le cree
  if (trie == NULL) {
    trie = creer_noeud();
  }

  int index;
  int len = strlen(str);
  TrieNoeud *noeud_temporaire = trie;

  // On insere les characteres un par un
  for (int i = 0; i < len; i++) {
    index = str[i];

    if (noeud_temporaire->fils[index] == NULL) {
      noeud_temporaire->fils[index] = creer_noeud();
    }

    noeud_temporaire = noeud_temporaire->fils[index];
  }

  // L'enchainement de caracteres est maintenant valide
  noeud_temporaire->est_cle_valide = true;

  return;
};

bool recherche_trie(TrieNoeud *trie, const char *str) {
  if (trie == NULL) {
    return false;
  }

  int len = strlen(str);
  unsigned int char_temporaire;
  TrieNoeud *noeud_temporaire = trie;

  // Parcourir les characteres un par un
  for (int i = 0; i < len; i++) {
    char_temporaire = str[i];
    // On a trouve null alors que la chaine de caracteres n'est pas fini
    if (noeud_temporaire->fils[char_temporaire] == NULL) {
      return false;
    }
    noeud_temporaire = noeud_temporaire->fils[char_temporaire];
  }

  // Si le dernier charactere existe et qu'il est valide, on a trouve
  return (noeud_temporaire != NULL && noeud_temporaire->est_cle_valide);
};

void liberer_trie(TrieNoeud *trie) {
  // Liberer chaque noeud recursivement
  for (int i = 0; i < NOMBRE_FILS; i++) {
    if (trie->fils[i]) {
      liberer_trie(trie->fils[i]);
    }
  }

  free(trie);

  return;
}