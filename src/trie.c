#include "../include/trie.h"

#include <limits.h>
#include <stdlib.h>
#include <string.h>

TrieNoeud creer_noeud_trie() {
  // On alloue l'espace memoire pour le noeud
  TrieNoeud nouveau_noeud = malloc(sizeof(struct trie_noeud));

  // On verifie que malloc n'a pas echoué
  if (nouveau_noeud == NULL) {
    // Print error to stderr
    fprintf(stderr, "Erreur d'allocation de mémoire\n");
    exit(1);
  }

  // On initialise le noeud et ses fils à NULL
  nouveau_noeud->est_cle_valide = false;
  for (size_t i = 0; i < NOMBRE_FILS; i++) {
    nouveau_noeud->fils[i] = NULL;
  }
  nouveau_noeud->valeur = NULL;

  return nouveau_noeud;
};

TrieNoeud inserer_trie(TrieNoeud trie, char *cle, char *valeur) {
  // Si le trie n'existe pas, on le crée
  if (trie == NULL) {
    trie = creer_noeud_trie();
  }

  // On itére sur les fils et on insére chaque caractère de la clé
  unsigned int index;
  unsigned int len = strlen(cle);
  TrieNoeud noeud_temporaire = trie;

  // On insere les caractères un par un
  for (unsigned int i = 0; i < len; i++) {
    index = (unsigned char)cle[i];

    if (noeud_temporaire->fils[index] == NULL) {
      noeud_temporaire->fils[index] = creer_noeud_trie();
    }

    noeud_temporaire = noeud_temporaire->fils[index];
  }

  // L'enchainement de caractères est maintenant valide
  noeud_temporaire->est_cle_valide = true;
  noeud_temporaire->valeur = malloc(strlen(valeur) + 1);
  strcpy(noeud_temporaire->valeur, valeur);

  return trie;
}

char *recuperer_trie(TrieNoeud trie, char *cle) {
  if (trie == NULL) {
    return "NULL";
  }

  unsigned int len = strlen(cle);
  unsigned int char_temporaire;
  TrieNoeud noeud_temporaire = trie;

  // Parcourir les caracteres de la clé un par un
  for (unsigned int i = 0; i < len; i++) {
    char_temporaire = (unsigned char)cle[i];
    // On a trouve NULL alors que la chaine de caracteres n'est pas fini
    if (noeud_temporaire->fils[char_temporaire] == NULL) {
      return "NULL";
    }
    noeud_temporaire = noeud_temporaire->fils[char_temporaire];
  }

  // Si le dernier caractere existe et qu'il est valide, on le renvoie
  if (noeud_temporaire != NULL && noeud_temporaire->est_cle_valide) {
    return noeud_temporaire->valeur;
  } else {
    return "NULL";
  }
};

void liberer_trie(TrieNoeud trie) {
  if (trie == NULL) {
    return;
  }

  // On libere tous les fils de tous les noeuds avec un parcours en profondeur
  for (int i = 0; i < NOMBRE_FILS; i++) {
    liberer_trie(trie->fils[i]);
  }

  if (trie->valeur) {
    free(trie->valeur);
  }

  free(trie);
}
