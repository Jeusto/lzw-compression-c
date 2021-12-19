#include "../include/liste.h"

#include <stdlib.h>
#include <string.h>

ListeNoeud creer_noeud_liste(char *cle, char *valeur) {
  // On alloue l'espace memoire pour le noeud
  ListeNoeud nouveau_noeud = malloc(sizeof(struct liste_noeud));

  // On verifie que malloc n'a pas echoué
  if (nouveau_noeud == NULL) {
    fprintf(stderr, "Erreur d'allocation de mémoire\n");
    exit(1);
  }

  // On initialise le noeud avec ses valeurs
  nouveau_noeud->cle = malloc(strlen(cle) + 1);
  strcpy(nouveau_noeud->cle, cle);
  nouveau_noeud->valeur = malloc(strlen(valeur) + 1);
  strcpy(nouveau_noeud->valeur, valeur);
  nouveau_noeud->prochain = NULL;

  return nouveau_noeud;
}

ListeNoeud inserer_liste(ListeNoeud liste, char *cle, char *valeur) {
  // Si la liste n'existe pas, on le crée
  if (liste == NULL) {
    liste = creer_noeud_liste(cle, valeur);
  }

  else {
    // On insere le nouveau noeud en fin de liste
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
  if (liste == NULL) {
    return "NULL";
  }

  ListeNoeud noeud = liste;

  while (noeud != NULL) {
    // On compare la clé, si elle est trouvée,  on renvoie la valeur
    if (strcmp(noeud->cle, cle) == 0) {
      return noeud->valeur;
    }
    noeud = noeud->prochain;
  }
  return "NULL";
}

void liberer_liste(ListeNoeud liste) {
  ListeNoeud actuel = liste;
  ListeNoeud prochain;

  // On libere tous les noeuds en parcourant la liste
  while (actuel != NULL) {
    prochain = actuel->prochain;
    free(actuel->cle);
    free(actuel->valeur);
    free(actuel);
    actuel = prochain;
  }
}