#include "../include/lzw.h"

TrieNoeud initialiser_trie(char* mode) {
  TrieNoeud dict = NULL;

  if (strcmp(mode, "compression") == 0) {
    for (int i = 0; i < 256; i++) {
      char cle[TAILLE_MAX_STRING] = "";
      char valeur[TAILLE_MAX_HEXA_STRING] = "";
      char* charstr = char2str(i);
      strcpy(cle, charstr);
      free(charstr);
      sprintf(valeur, "%08X", i);
      dict = inserer_trie(dict, cle, valeur);
    }
  } else {
    for (int i = 0; i < 256; i++) {
      char cle[TAILLE_MAX_HEXA_STRING] = "";
      char valeur[TAILLE_MAX_STRING] = "";
      sprintf(cle, "%08X", i);
      char* charstr = char2str(i);
      strcpy(valeur, charstr);
      free(charstr);
      dict = inserer_trie(dict, cle, valeur);
    }
  }
  return dict;
}

void compresser_trie(TrieNoeud dict, FILE* fichier_source,
                     FILE* fichier_destination) {
  // Variables utiles pour l'algorithme
  int taille_actuelle_dico = 255;
  char cle_P[TAILLE_MAX_STRING] = "";
  char lecture_char_C;
  char cle_C[TAILLE_MAX_STRING] = "";
  char cle_P_plus_C[TAILLE_MAX_STRING] = "";
  char valeur_P[TAILLE_MAX_HEXA_STRING] = "";
  char valeur_P_plus_C[TAILLE_MAX_HEXA_STRING] = "";

  // Algorithme de compression

  /* P = first input character */
  char* concatstr = char2str(fgetc(fichier_source));
  strcpy(cle_P, concatstr);
  strcpy(valeur_P, recuperer_trie(dict, cle_P));
  free(concatstr);

  /* While not end of input stream*/
  /* C = next input character */
  while ((lecture_char_C = fgetc(fichier_source)) != EOF) {
    concatstr = char2str(lecture_char_C);
    strcpy(cle_C, concatstr);
    free(concatstr);

    /* If P+C is in the string table */
    concatstr = concat(cle_P, cle_C);
    strcpy(cle_P_plus_C, concatstr);
    free(concatstr);

    if ((strcmp(recuperer_trie(dict, cle_P_plus_C), "NULL")) != 0) {
      strcpy(valeur_P_plus_C, recuperer_trie(dict, cle_P_plus_C));

      /* P = P + C */
      concatstr = concat(cle_P, cle_C);
      strcpy(cle_P, concatstr);
      free(concatstr);

      /* Else */
    } else {
      printf("P+C = %s is not in the string table\n", cle_P_plus_C);

      /* Output the code for P */
      strcpy(valeur_P, recuperer_trie(dict, cle_P));
      wb_hex_as_short(fichier_destination, valeur_P);
      printf("Output the code for P : %s : %s\n", cle_P, valeur_P);

      /* Add P+C to the string table */
      taille_actuelle_dico++;
      sprintf(valeur_P_plus_C, "%08X", taille_actuelle_dico);
      printf("Add P+C (%s : %s) to the string table at index (%d)\n",
             cle_P_plus_C, valeur_P_plus_C, taille_actuelle_dico);
      dict = inserer_trie(dict, cle_P_plus_C, valeur_P_plus_C);

      /* P = C */
      strcpy(cle_P, cle_C);
    }
    if (taille_actuelle_dico == TAILLE_DICT - 2) {
      /* Output code for P */
      strcpy(valeur_P, recuperer_trie(dict, cle_P));
      wb_hex_as_short(fichier_destination, valeur_P);
      printf("Output code for P = %s\n", cle_P);

      printf("DICTIONNAIRE PLEIN\n");
      liberer_trie(dict);
      dict = initialiser_trie("compression");
      compresser_trie(dict, fichier_source, fichier_destination);
      return;
    }
  }

  /* Output code for P */
  strcpy(valeur_P, recuperer_trie(dict, cle_P));
  wb_hex_as_short(fichier_destination, valeur_P);
  printf("Output code for P = %s\n", cle_P);
  liberer_trie(dict);
  return;
}

void decompresser_trie(TrieNoeud dict, FILE* fichier_source,
                       FILE* fichier_destination) {
  // Variables utiles pour l'algorithme

  int taille_actuelle_dico = 255;
  char cle_Old[TAILLE_MAX_HEXA_STRING] = "";
  char valeur_Old[TAILLE_MAX_STRING] = "";
  char cle_New[TAILLE_MAX_HEXA_STRING] = "";
  char valeur_New[TAILLE_MAX_STRING] = "";
  char valeur_S[TAILLE_MAX_STRING] = "";
  char char_C = 0;

  // Algorithme de decompression

  /* OLD = first input code */
  printf("🤬 Debut\n");
  char* charstr = rb_next_short_as_hex(fichier_source);
  strcpy(cle_Old, charstr);
  free(charstr);
  strcpy(valeur_Old, recuperer_trie(dict, cle_Old));
  printf("📍OLD = first input code : %s => %s\n", cle_Old, valeur_Old);

  /* Output translation of OLD */
  printf("🛂 Output valeur de OLD\n");
  fprintf(fichier_destination, "%s", valeur_Old);

  /* While not end of input stream */
  int iteration = 0;
  while (1) {
    printf("🏅 Iteration = %d\n", iteration++);

    if (taille_actuelle_dico == TAILLE_DICT - 2) {
      printf("📍 DICTIONNAIRE PLEIN\n");
      liberer_trie(dict);
      dict = initialiser_trie("decompression");
      decompresser_trie(dict, fichier_source, fichier_destination);
      return;
    }

    /* NEW = Next input code */
    char* new = rb_next_short_as_hex(fichier_source);
    if (new == NULL) {
      liberer_trie(dict);
      return;  // Fin
    }
    strcpy(cle_New, new);
    free(new);
    printf("NEW = next input code : %s : %s\n", cle_New,
           recuperer_trie(dict, cle_New));
    if (strcmp(recuperer_trie(dict, cle_New), "NULL") == 0) {
      printf("🛂  NEW is not in the string table\n");
      /* S = translation of OLD + C */
      char* charstr2 = char2str(char_C);
      charstr = concat(valeur_Old, charstr);
      strcpy(valeur_S, charstr);
      free(charstr2);
      free(charstr);
      printf("S = translation of OLD + C : %s\n", valeur_S);
    } else {
      printf("🛂  NEW is in the string table\n");
      /* S = translation of NEW */
      strcpy(valeur_New, recuperer_trie(dict, cle_New));
      strcpy(valeur_S, valeur_New);
      printf("S = translation of NEW : . => %s\n", valeur_S);
    }

    /* Output S */
    printf("Output S : . => %s\n", valeur_S);
    fprintf(fichier_destination, "%s", valeur_S);

    /* C = first character of S */
    char_C = valeur_S[0];
    printf("C = premier char de S : %c\n", char_C);

    /* OLD + C to the string table */
    taille_actuelle_dico++;

    char cle_old_c[TAILLE_MAX_HEXA_STRING] = "";
    sprintf(cle_old_c, "%08X", taille_actuelle_dico);
    char* charstr2 = char2str(char_C);
    charstr = concat(valeur_Old, charstr2);
    dict = inserer_trie(dict, cle_old_c, charstr);

    printf("OLD + C  (%s : %s), ajouter a la table a l'indice %d\n", cle_old_c,
           charstr, taille_actuelle_dico);
    free(charstr2);
    free(charstr);

    /* OLD = New */
    strcpy(cle_Old, cle_New);
    strcpy(valeur_Old, recuperer_trie(dict, cle_Old));
    printf("OLD = NEW : %s => %s\n", cle_Old, valeur_Old);
  }
}

void liberer_dictionnaire_hashmap(struct hashmap_s* dict, char** tableau_cles,
                                  char** tableau_valeurs) {
  for (int i = 0; i < TAILLE_DICT; i++) {
    free(tableau_cles[i]);
    free(tableau_valeurs[i]);
  }
  free(tableau_cles);
  free(tableau_valeurs);
  hashmap_destroy(dict);
}

struct hashmap_s initialiser_hashmap(char* mode, char** tableau_cles,
                                     char** tableau_valeurs) {
  struct hashmap_s dict;
  if (0 != hashmap_create(TAILLE_DICT, &dict)) {
    raler(0, "Erreur: creation hashmap\n");
  }

  if (strcmp(mode, "compression") == 0) {
    for (int i = 0; i <= 255; i++) {
      char* charstr = char2str(i);
      strcpy(tableau_cles[i], charstr);
      free(charstr);

      sprintf(tableau_valeurs[i], "%08X", i);

      if (0 != hashmap_put(&dict, tableau_cles[i], strlen(tableau_cles[i]),
                           tableau_valeurs[i])) {
        raler(0, "Erreur: insertion dans hashmap\n");
      }
    }
  } else {
    for (int i = 0; i <= 255; i++) {
      sprintf(tableau_cles[i], "%08X", i);
      char* charstr = char2str(i);
      strcpy(tableau_valeurs[i], charstr);
      free(charstr);

      if (0 != hashmap_put(&dict, tableau_cles[i], strlen(tableau_cles[i]),
                           tableau_valeurs[i])) {
        raler(0, "Erreur: insertion dans hashmap\n");
      }
    }
  }
  return dict;
}

void compresser_hashmap(FILE* fichier_source, FILE* fichier_destination,
                        struct hashmap_s dict, char** tableau_cles,
                        char** tableau_valeurs) {
  int taille_actuelle_dico = 255;

  // Variables utiles pour l'algorithme
  char cle_P[TAILLE_MAX_STRING] = "";
  char lecture_char_C;
  char cle_C[TAILLE_MAX_STRING] = "";
  char cle_P_plus_C[TAILLE_MAX_STRING] = "";
  char* valeur_P;
  char* valeur_P_plus_C;

  /* P = first input character */
  char* charstr = char2str(fgetc(fichier_source));
  strcpy(cle_P, charstr);
  free(charstr);
  valeur_P = hashmap_get(&dict, cle_P, strlen(cle_P));

  /* While not end of input stream*/
  /* C = next input character */
  while ((lecture_char_C = fgetc(fichier_source)) != EOF) {
    charstr = char2str(lecture_char_C);
    strcpy(cle_C, charstr);
    free(charstr);

    /* If P+C is in the string table */

    charstr = concat(cle_P, cle_C);
    strcpy(cle_P_plus_C, charstr);
    free(charstr);
    valeur_P_plus_C = hashmap_get(&dict, cle_P_plus_C, strlen(cle_P_plus_C));
    if (valeur_P_plus_C) {
      printf("P+C = %s is in the string table : %s \n", cle_P_plus_C,
             valeur_P_plus_C);

      /* P = P + C */
      charstr = concat(cle_P, cle_C);
      strcpy(cle_P, charstr);
      free(charstr);

      /* Else */
    } else {
      printf("P+C = %s is not in the string table\n", cle_P_plus_C);

      /* Output the code for P */
      valeur_P = hashmap_get(&dict, cle_P, strlen(cle_P));
      wb_hex_as_short(fichier_destination, valeur_P);
      printf("Output the code for P : %s : %s\n", cle_P, valeur_P);

      /* Add P+C to the string table */
      taille_actuelle_dico++;
      sprintf(tableau_valeurs[taille_actuelle_dico], "%08X",
              taille_actuelle_dico);
      strcpy(tableau_cles[taille_actuelle_dico], cle_P_plus_C);
      if (0 != hashmap_put(&dict, tableau_cles[taille_actuelle_dico],
                           strlen(tableau_cles[taille_actuelle_dico]),
                           tableau_valeurs[taille_actuelle_dico])) {
        raler(0, "Erreur: insertion dans hashmap\n");
      }
      printf("Add P+C (%s : %s) to the string table at index (%d)\n",
             tableau_cles[taille_actuelle_dico],
             tableau_valeurs[taille_actuelle_dico], taille_actuelle_dico);

      /* P = C */
      strcpy(cle_P, cle_C);
    }

    if (taille_actuelle_dico == TAILLE_DICT - 2) {
      /* Output code for P */
      strcpy(valeur_P, hashmap_get(&dict, cle_P, strlen(cle_P)));
      wb_hex_as_short(fichier_destination, valeur_P);
      printf("Output code for P = %s\n", cle_P);

      printf("DICTIONNAIRE MAX\n");
      liberer_dictionnaire_hashmap(&dict, tableau_cles, tableau_valeurs);
      char** tableau_cles = malloc(TAILLE_DICT * TAILLE_MAX_STRING);
      for (int i = 0; i < TAILLE_DICT; i++) {
        tableau_cles[i] = malloc(TAILLE_MAX_STRING);
      }
      char** tableau_valeurs = malloc(TAILLE_DICT * TAILLE_MAX_HEXA_STRING);
      for (int i = 0; i < TAILLE_DICT; i++) {
        tableau_valeurs[i] = malloc(TAILLE_MAX_HEXA_STRING);
      }
      dict = initialiser_hashmap("compression", tableau_cles, tableau_valeurs);

      compresser_hashmap(fichier_source, fichier_destination, dict,
                         tableau_cles, tableau_valeurs);

      return;
    }
  }

  /* Output code for P */
  strcpy(valeur_P, hashmap_get(&dict, cle_P, strlen(cle_P)));
  wb_hex_as_short(fichier_destination, valeur_P);
  printf("Output code for P = %s\n", cle_P);
  liberer_dictionnaire_hashmap(&dict, tableau_cles, tableau_valeurs);
  return;
}

void decompresser_hashmap(FILE* fichier_source, FILE* fichier_destination,
                          struct hashmap_s dict, char** tableau_cles,
                          char** tableau_valeurs) {
  // Initialiser le dictionnaire avec les 256 premiers cles/valeurs
  int taille_actuelle_dico = 255;

  // Variables utiles pour l'algorithme
  char cle_Old[TAILLE_MAX_HEXA_STRING] = "";
  char* valeur_Old;
  char cle_New[TAILLE_MAX_HEXA_STRING] = "";
  char* valeur_New;
  char valeur_S[TAILLE_MAX_STRING] = "";
  char char_C = '0';

  // Algorithme de decompression

  /* OLD = first input code */
  printf("🤬 Debut\n");
  char* charstr = rb_next_short_as_hex(fichier_source);
  strcpy(cle_Old, charstr);
  free(charstr);
  valeur_Old = hashmap_get(&dict, cle_Old, strlen(cle_Old));
  printf("📍OLD = first input code : %s => %s\n", cle_Old, valeur_Old);

  /* Output translation of OLD */
  printf("🛂 Output valeur de OLD\n");
  fprintf(fichier_destination, "%s", valeur_Old);

  /* While not end of input stream */
  int iteration = 0;
  while (1) {
    printf("🏅 Iteration = %d\n", iteration++);

    if (taille_actuelle_dico == TAILLE_DICT - 2) {
      printf("📍 DICTIONNAIRE PLEIN\n");
      liberer_dictionnaire_hashmap(&dict, tableau_cles, tableau_valeurs);
      char** tableau_cles = malloc(TAILLE_DICT * TAILLE_MAX_HEXA_STRING);
      for (int i = 0; i < TAILLE_DICT; i++) {
        tableau_cles[i] = malloc(TAILLE_MAX_HEXA_STRING);
      }
      char** tableau_valeurs = malloc(TAILLE_DICT * TAILLE_MAX_STRING);
      for (int i = 0; i < TAILLE_DICT; i++) {
        tableau_valeurs[i] = malloc(TAILLE_MAX_STRING);
      }
      dict =
          initialiser_hashmap("decompression", tableau_cles, tableau_valeurs);

      decompresser_hashmap(fichier_source, fichier_destination, dict,
                           tableau_cles, tableau_valeurs);
      return;
    }

    /* NEW = Next input code */
    char* new = rb_next_short_as_hex(fichier_source);
    if (new == NULL) {
      liberer_dictionnaire_hashmap(&dict, tableau_cles, tableau_valeurs);
      return;  // Fin
    }
    strcpy(cle_New, new);
    free(new);
    valeur_New = hashmap_get(&dict, cle_New, strlen(cle_New));
    printf("NEW = next input code : %s : %s\n", cle_New, valeur_New);
    if (hashmap_get(&dict, cle_New, strlen(cle_New)) == NULL) {
      printf("🛂  NEW is not in the string table\n");
      /* S = translation of OLD + C */
      char* charstr2 = char2str(char_C);
      charstr = concat(valeur_Old, charstr);
      strcpy(valeur_S, charstr);
      free(charstr2);
      free(charstr);
      printf("S = translation of OLD + C : %s\n", valeur_S);
    } else {
      printf("🛂  NEW is in the string table\n");
      /* S = translation of NEW */
      valeur_New = hashmap_get(&dict, cle_New, strlen(cle_New));
      strcpy(valeur_S, valeur_New);
      printf("S = translation of NEW : . => %s\n", valeur_S);
    }

    printf("Output S : . => %s\n", valeur_S);
    fprintf(fichier_destination, "%s", valeur_S);

    /* C = first character of S */
    char_C = valeur_S[0];
    printf("C = premier char de S : %c\n", char_C);

    /* OLD + C to the string table */
    taille_actuelle_dico++;

    sprintf(tableau_cles[taille_actuelle_dico], "%08X", taille_actuelle_dico);
    char* charstr2 = char2str(char_C);
    charstr = concat(valeur_Old, charstr2);
    strcpy(tableau_valeurs[taille_actuelle_dico], charstr);
    free(charstr2);
    free(charstr);

    if (0 != hashmap_put(&dict, tableau_cles[taille_actuelle_dico],
                         strlen(tableau_cles[taille_actuelle_dico]),
                         tableau_valeurs[taille_actuelle_dico])) {
      raler(0, "Erreur: insertion dans hashmap\n");
    }
    /* OLD = New */
    strcpy(cle_Old, cle_New);
    valeur_Old = hashmap_get(&dict, cle_Old, strlen(cle_Old));
    printf("OLD = NEW : %s => %s\n", cle_Old, valeur_Old);
  }
}

ListeNoeud initialiser_liste(char* mode) {
  ListeNoeud dict = NULL;

  if (strcmp(mode, "compression") == 0) {
    for (int i = 0; i < 256; i++) {
      char cle[TAILLE_MAX_STRING] = "";
      char valeur[TAILLE_MAX_HEXA_STRING] = "";
      sprintf(valeur, "%08X", i);
      char* charstr = char2str(i);
      strcpy(cle, charstr);
      free(charstr);
      dict = inserer_liste(dict, cle, valeur);
    }
  } else {
    for (int i = 0; i < 256; i++) {
      char cle[TAILLE_MAX_HEXA_STRING] = "";
      char valeur[TAILLE_MAX_STRING] = "";
      sprintf(cle, "%08X", i);
      char* charstr = char2str(i);
      strcpy(valeur, charstr);
      free(charstr);
      dict = inserer_liste(dict, cle, valeur);
    }
  }

  return dict;
}

void compresser_liste(ListeNoeud dict, FILE* fichier_source,
                      FILE* fichier_destination) {
  // Variables utiles pour l'algorithme

  int taille_actuelle_dico = 255;
  char cle_P[TAILLE_MAX_STRING] = "";
  char lecture_char_C;
  char cle_C[TAILLE_MAX_STRING] = "";
  char cle_P_plus_C[TAILLE_MAX_STRING] = "";
  char valeur_P[TAILLE_MAX_HEXA_STRING] = "";
  char valeur_P_plus_C[TAILLE_MAX_HEXA_STRING] = "";

  // Algorithme de compression

  /* P = first input character */
  char* concatstr = char2str(fgetc(fichier_source));
  strcpy(cle_P, concatstr);
  strcpy(valeur_P, recuperer_liste(dict, cle_P));
  free(concatstr);

  /* While not end of input stream*/
  /* C = next input character */
  while ((lecture_char_C = fgetc(fichier_source)) != EOF) {
    concatstr = char2str(lecture_char_C);
    strcpy(cle_C, concatstr);
    free(concatstr);

    /* If P+C is in the string table */
    concatstr = concat(cle_P, cle_C);
    strcpy(cle_P_plus_C, concatstr);
    free(concatstr);

    if ((strcmp(recuperer_liste(dict, cle_P_plus_C), "NULL")) != 0) {
      strcpy(valeur_P_plus_C, recuperer_liste(dict, cle_P_plus_C));

      /* P = P + C */
      concatstr = concat(cle_P, cle_C);
      strcpy(cle_P, concatstr);
      free(concatstr);

      /* Else */
    } else {
      /* Output the code for P */
      strcpy(valeur_P, recuperer_liste(dict, cle_P));
      wb_hex_as_short(fichier_destination, valeur_P);

      /* Add P+C to the string table */
      taille_actuelle_dico++;
      sprintf(valeur_P_plus_C, "%08X", taille_actuelle_dico);
      printf("Add P+C (%s : %s) to the string table at index (%d)\n",
             cle_P_plus_C, valeur_P_plus_C, taille_actuelle_dico);
      dict = inserer_liste(dict, cle_P_plus_C, valeur_P_plus_C);

      /* P = C */
      strcpy(cle_P, cle_C);
    }
    if (taille_actuelle_dico == TAILLE_DICT - 2) {
      /* Output code for P */
      strcpy(valeur_P, recuperer_liste(dict, cle_P));
      wb_hex_as_short(fichier_destination, valeur_P);
      printf("Output code for P = %s\n", cle_P);

      printf("DICTIONNAIRE PLEIN\n");
      liberer_liste(dict);
      dict = initialiser_liste("compression");
      compresser_liste(dict, fichier_source, fichier_destination);
      return;
    }
  }

  /* Output code for P */
  strcpy(valeur_P, recuperer_liste(dict, cle_P));
  wb_hex_as_short(fichier_destination, valeur_P);
  printf("Output code for P = %s\n", cle_P);
  liberer_liste(dict);
}

void decompresser_liste(ListeNoeud dict, FILE* fichier_source,
                        FILE* fichier_destination) {
  // Variables utiles pour l'algorithme

  int taille_actuelle_dico = 255;
  char cle_Old[TAILLE_MAX_HEXA_STRING] = "";
  char valeur_Old[TAILLE_MAX_STRING] = "";
  char cle_New[TAILLE_MAX_HEXA_STRING] = "";
  char valeur_New[TAILLE_MAX_STRING] = "";
  char valeur_S[TAILLE_MAX_STRING] = "";
  char char_C = '0';

  // Algorithme de decompression

  /* OLD = first input code */
  printf("🤬 Debut\n");
  char* charstr = rb_next_short_as_hex(fichier_source);
  strcpy(cle_Old, charstr);
  free(charstr);
  strcpy(valeur_Old, recuperer_liste(dict, cle_Old));
  printf("📍OLD = first input code : %s => %s\n", cle_Old, valeur_Old);

  /* Output translation of OLD */
  printf("🛂 Output valeur de OLD\n");
  fprintf(fichier_destination, "%s", valeur_Old);

  /* While not end of input stream */
  int iteration = 0;
  while (1) {
    printf("🏅 Iteration = %d\n", iteration++);

    if (taille_actuelle_dico == TAILLE_DICT - 2) {
      printf("📍 DICTIONNAIRE PLEIN\n");
      liberer_liste(dict);
      dict = initialiser_liste("decompression");
      decompresser_liste(dict, fichier_source, fichier_destination);
      return;
    }

    /* NEW = Next input code */
    char* new = rb_next_short_as_hex(fichier_source);
    if (new == NULL) {
      liberer_liste(dict);
      return;  // Fin
    }
    strcpy(cle_New, new);
    free(new);
    printf("NEW = next input code : %s : %s\n", cle_New,
           recuperer_liste(dict, cle_New));
    if (strcmp(recuperer_liste(dict, cle_New), "NULL") == 0) {
      printf("🛂  NEW is not in the string table\n");
      /* S = translation of OLD + C */
      char* charstr2 = char2str(char_C);
      charstr = concat(valeur_Old, charstr);
      strcpy(valeur_S, charstr);
      free(charstr2);
      free(charstr);
      printf("S = translation of OLD + C : %s\n", valeur_S);
    } else {
      printf("🛂  NEW is in the string table\n");
      /* S = translation of NEW */
      strcpy(valeur_New, recuperer_liste(dict, cle_New));
      strcpy(valeur_S, valeur_New);
      printf("S = translation of NEW : . => %s\n", valeur_S);
    }

    /* Output S */
    printf("Output S : . => %s\n", valeur_S);
    fprintf(fichier_destination, "%s", valeur_S);

    /* C = first character of S */
    char_C = valeur_S[0];
    printf("C = premier char de S : %c\n", char_C);

    /* OLD + C to the string table */
    taille_actuelle_dico++;

    char cle_old_c[TAILLE_MAX_HEXA_STRING] = "";
    sprintf(cle_old_c, "%08X", taille_actuelle_dico);
    char* charstr2 = char2str(char_C);
    charstr = concat(valeur_Old, charstr2);
    dict = inserer_liste(dict, cle_old_c, charstr);

    printf("OLD + C  (%s : %s), ajouter a la table a l'indice %d\n", cle_old_c,
           charstr, taille_actuelle_dico);
    free(charstr2);
    free(charstr);

    /* OLD = New */
    strcpy(cle_Old, cle_New);
    strcpy(valeur_Old, recuperer_liste(dict, cle_Old));
    printf("OLD = NEW : %s => %s\n", cle_Old, valeur_Old);
  }
}
