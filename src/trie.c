#include "trie.h"

#include <stdlib.h>
#include <string.h>

Noeud_Trie *creer_noeud() {
  Noeud_Trie *nouveau_noeud = malloc(sizeof(struct Noeud_Trie));

  if (nouveau_noeud == NULL) {
    printf("erreur");
    exit(1);
  }

  nouveau_noeud->est_cle_valide = false;
  for (size_t i = 0; i < NOMBRE_FILS; i++) {
    nouveau_noeud->fils[i] = NULL;
  }

  return nouveau_noeud;
};

void insertion_trie(Noeud_Trie *trie, const char *str) {
  if (trie == NULL) {
    trie = creer_noeud();
  }

  int index;
  int len = strlen(str);
  Noeud_Trie *noeud_temporaire;

  for (int i = 0; i < len; i++) {
    index = str[i];

    if (noeud_temporaire->fils[index] == NULL) {
      noeud_temporaire->fils[index] = creer_noeud();
    }

    noeud_temporaire = noeud_temporaire->fils[index];
  }

  noeud_temporaire->est_cle_valide = true;

  return;
};

bool recherche_trie(Noeud_Trie *trie, const char *str) {
  if (trie == NULL) {
    return false;
  }

  int len = strlen(str);
  char char_temporaire;
  Noeud_Trie *noeud_temporaire = trie;

  for (int i = 0; i < len; i++) {
    char_temporaire = str[i];
    if (noeud_temporaire->fils[char_temporaire] == NULL) {
      return false;
    }
    noeud_temporaire = noeud_temporaire->fils[char_temporaire];
  }

  return true;
};

void liberer_trie(Noeud_Trie *trie) {
  for (int i = 0; i < NOMBRE_FILS; i++) {
    if (trie->fils[i]) {
      liberer_trie(trie->fils[i]);
    }
  }

  free(trie);

  return;
}