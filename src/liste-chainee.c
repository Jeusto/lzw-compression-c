#include "../include/liste-chainee.h"

#include <stdlib.h>
#include <string.h>

void liberer_liste(ListeNoeud liste) {
  ListeNoeud actuel = liste;
  ListeNoeud prochain;
  while (actuel != NULL) {
    prochain = actuel->prochain;
    free(actuel->cle);
    free(actuel->valeur);
    free(actuel);
    actuel = prochain;
  }
}

ListeNoeud creer_noeud_liste(char *cle, char *valeur) {
  ListeNoeud nouveau_noeud = malloc(sizeof(struct liste_noeud));
  if (nouveau_noeud == NULL) {
    printf("erreur");
    exit(1);
  }
  nouveau_noeud->cle = malloc(strlen(cle) + 1);
  strcpy(nouveau_noeud->cle, cle);
  nouveau_noeud->valeur = malloc(strlen(valeur) + 1);
  strcpy(nouveau_noeud->valeur, valeur);
  nouveau_noeud->prochain = NULL;

  return nouveau_noeud;
}

ListeNoeud inserer_liste(ListeNoeud liste, char *cle, char *valeur) {
  if (liste == NULL) {
    liste = creer_noeud_liste(cle, valeur);
  }

  else {
    ListeNoeud noeud = (struct liste_noeud *)malloc(sizeof(struct liste_noeud));
    noeud->cle = malloc(strlen(cle) + 1);
    strcpy(noeud->cle, cle);
    noeud->valeur = malloc(strlen(valeur) + 1);
    strcpy(noeud->valeur, valeur);
    noeud->prochain = NULL;

    ListeNoeud dernier = liste;
    while (dernier->prochain != NULL) {
      dernier = dernier->prochain;
    }
    dernier->prochain = noeud;
  }

  return liste;
}

char *recuperer_liste(ListeNoeud liste, char *cle) {
  ListeNoeud noeud = liste;
  while (noeud != NULL) {
    if (strcmp(noeud->cle, cle) == 0) {
      return noeud->valeur;
    }
    noeud = noeud->prochain;
  }
  return "NULL";
}
