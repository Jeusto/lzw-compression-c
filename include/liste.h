#ifndef __LISTE_H
#define __LISTE_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct liste_noeud {
  char *cle;
  char *valeur;
  struct liste_noeud *prochain;
} * ListeNoeud;

/*
 * @brief Crée un nouveau noeud de liste chainée.
 *
 * @param clé: la nouvelle clé à insérer
 * @param valeur: la valeur associée à la clé
 *
 * @return le nouveau noeud
 */
ListeNoeud creer_noeud_liste(char *cle, char *valeur);

/*
 * @brief Insère dans la liste chainée un couple clé/valeur.
 *
 * @param liste: la liste à parcourir
 * @param clé: la clé qui permet de rechercher un couple clé/valeur
 * @param valeur: la valeur associée à la clé
 *
 * @return la liste chainée avec le nouveau couple clé/valeur
 */
ListeNoeud inserer_liste(ListeNoeud liste, char *cle, char *valeur);

/*
 * @brief Recherche dans la liste chainée une clé et renvoie la valeur
 * associée.
 *
 * @param liste: la liste à parcourir
 * @param clé: la clé à rechercher
 *
 * @return la valeur associée à la clé
 */
char *recuperer_liste(ListeNoeud liste, char *cle);

/*
 * @brief Libére l'emplacement mémoire associée à une liste chainée.
 *
 * @param liste: la liste à libérer
 */
void liberer_liste(ListeNoeud liste);

#endif