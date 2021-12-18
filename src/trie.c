#include "../include/trie.h"

#include <limits.h>
#include <stdlib.h>
#include <string.h>

void liberer_trie(TrieNoeud trie) {
  if (trie == NULL) {
    return;
  }

  for (int i = 0; i < NOMBRE_FILS; i++) {
    liberer_trie(trie->fils[i]);
  }

  if (trie->valeur) {
    free(trie->valeur);
  }
  free(trie);
}

TrieNoeud creer_noeud() {
  // On alloue l'espace memoire pour le noeud
  TrieNoeud nouveau_noeud = malloc(sizeof(struct trie_noeud));

  if (nouveau_noeud == NULL) {
    printf("erreur");
    exit(1);
  }

  // Initialiser le noeud et ses fils
  nouveau_noeud->est_cle_valide = false;
  for (size_t i = 0; i < NOMBRE_FILS; i++) {
    nouveau_noeud->fils[i] = NULL;
  }
  nouveau_noeud->valeur = NULL;

  return nouveau_noeud;
};

void inserer_trie(TrieNoeud trie, char *cle, char *valeur) {
  // TrieNoeud n'existe pas, on le cree
  if (trie == NULL) {
    trie = creer_noeud();
  }
  unsigned int index;
  unsigned int len = strlen(cle);
  TrieNoeud noeud_temporaire = trie;

  // On insere les characteres un par un
  for (unsigned int i = 0; i < len; i++) {
    index = (unsigned char)cle[i];

    if (noeud_temporaire->fils[index] == NULL) {
      noeud_temporaire->fils[index] = creer_noeud();
    }

    noeud_temporaire = noeud_temporaire->fils[index];
  }

  // L'enchainement de caracteres est maintenant valide
  noeud_temporaire->est_cle_valide = true;
  noeud_temporaire->valeur = malloc(strlen(valeur) + 1);
  strcpy(noeud_temporaire->valeur, valeur);
}

char *recuperer_trie(TrieNoeud trie, char *cle) {
  if (trie == NULL) {
    return false;
  }

  unsigned int len = strlen(cle);
  unsigned int char_temporaire;
  TrieNoeud noeud_temporaire = trie;

  // Parcourir les characteres un par un
  for (unsigned int i = 0; i < len; i++) {
    char_temporaire = (unsigned char)cle[i];
    // On a trouve null alors que la chaine de caracteres n'est pas fini
    if (noeud_temporaire->fils[char_temporaire] == NULL) {
      return "NULL";
    }
    noeud_temporaire = noeud_temporaire->fils[char_temporaire];
  }

  // Si le dernier charactere existe et qu'il est valide, on a trouve
  if (noeud_temporaire != NULL && noeud_temporaire->est_cle_valide) {
    return noeud_temporaire->valeur;
  } else {
    return "NULL";
  }
};

// void liberer_trie(TrieNoeud trie) {
//  Liberer chaque noeud recursivement
// for (int i = 0; i < NOMBRE_FILS; i++) {
//  if (trie->fils[i]) {
//   liberer_trie(trie->fils[i]);
//}
//}
// free(trie);
// return;
//}
