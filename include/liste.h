#ifndef __LISTE_H
#define __LISTE_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define TAILLE_MOT 4

typedef struct liste_noeud {
  char *cle;
  char *valeur;
  struct liste_noeud *prochain;
} * ListeNoeud;

void liberer_liste(ListeNoeud liste);
void afficher_liste(ListeNoeud liste);
ListeNoeud inserer_liste(ListeNoeud liste, char *cle, char *valeur);
char *recuperer_liste(ListeNoeud liste, char *cle);

#endif