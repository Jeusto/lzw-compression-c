#include "../include/main.h"

#define TAILLE_DICT 65536
#define TAILLE_MAX_HEXA_STRING 32
#define TAILLE_MAX_STRING 64

void compresser_trie(TrieNoeud dict, FILE* fichier_source,
                     FILE* fichier_destination) {
  // Variables utiles pour l'algorithme

  int taille_actuelle_dico = 255;
  char cle_P[TAILLE_MAX_STRING] = "";
  char lecture_char_C;
  char cle_C[TAILLE_MAX_STRING] = "";
  char cle_P_plus_C[TAILLE_MAX_STRING] = "";
  char valeur_P[TAILLE_MAX_STRING] = "";
  char valeur_P_plus_C[TAILLE_MAX_STRING] = "";

  // Algorithme de compression

  /* P = first input character */
  char* charstr = char2str(fgetc(fichier_source));
  strcpy(cle_P, charstr);
  strcpy(valeur_P, recuperer_trie(dict, cle_P));
  free(charstr);

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

    if ((strcmp(recuperer_trie(dict, cle_P_plus_C), "NULL")) != 0) {
      strcpy(valeur_P_plus_C, recuperer_trie(dict, cle_P_plus_C));
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
      strcpy(valeur_P, recuperer_trie(dict, cle_P));
      wb_hex_as_short(fichier_destination, valeur_P);
      printf("Output the code for P : %s : %s\n", cle_P, valeur_P);

      /* Add P+C to the string table */
      taille_actuelle_dico++;
      sprintf(valeur_P_plus_C, "%08X", taille_actuelle_dico);
      printf("Add P+C (%s : %s) to the string table at index (%d)\n",
             cle_P_plus_C, valeur_P_plus_C, taille_actuelle_dico);
      inserer_trie(dict, cle_P_plus_C, valeur_P_plus_C);

      /* P = C */
      strcpy(cle_P, cle_C);
    }
  }

  /* Output code for P */
  strcpy(valeur_P, recuperer_trie(dict, cle_P));
  wb_hex_as_short(fichier_destination, valeur_P);
  printf("Output code for P = %s\n", cle_P);
}

void decompresser_trie(TrieNoeud dict, FILE* fichier_source,
                       FILE* fichier_destination) {
  // Variables utiles pour l'algorithme

  int taille_actuelle_dico = 255;
  char cle_Old[TAILLE_MAX_HEXA_STRING] = "";
  char valeur_Old[TAILLE_MAX_HEXA_STRING] = "";
  char cle_New[TAILLE_MAX_HEXA_STRING] = "";
  char valeur_New[TAILLE_MAX_HEXA_STRING] = "";
  char valeur_S[TAILLE_MAX_HEXA_STRING] = "";
  char char_C = 0;

  // Algorithme de decompression

  /* OLD = first input code */
  printf("🤬 Debut\n");
  strcpy(cle_Old, rb_next_short_as_hex(fichier_source));
  strcpy(valeur_Old, recuperer_trie(dict, cle_Old));
  printf("📍OLD = first input code : %s => %s\n", cle_Old, valeur_Old);

  /* Output translation of OLD */
  printf("🛂 Output valeur de OLD\n");
  fprintf(fichier_destination, "%s", valeur_Old);

  /* While not end of input stream */
  int iteration = 0;
  while (1) {
    printf("🏅 Iteration = %d\n", iteration++);

    /* NEW = Next input code */
    char* new = rb_next_short_as_hex(fichier_source);
    if (new == NULL) return;  // Fin
    strcpy(cle_New, new);
    printf("NEW = next input code : %s : %s\n", cle_New,
           recuperer_trie(dict, cle_New));
    if (strcmp(recuperer_trie(dict, cle_New), "NULL") == 0) {
      printf("🛂  NEW is not in the string table\n");
      /* S = translation of OLD + C */
      strcpy(valeur_S, concat(valeur_Old, char2str(char_C)));
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
    inserer_trie(dict, cle_old_c, concat(valeur_Old, char2str(char_C)));

    printf("OLD + C  (%s : %s), ajouter a la table a l'indice %d\n", cle_old_c,
           concat(valeur_Old, char2str(char_C)), taille_actuelle_dico);

    /* OLD = New */
    strcpy(cle_Old, cle_New);
    strcpy(valeur_Old, recuperer_trie(dict, cle_Old));
    printf("OLD = NEW : %s => %s\n", cle_Old, valeur_Old);
  }
}

void compresser_hashmap(FILE* fichier_source, FILE* fichier_destination) {
  // Initialiser le dictionnaire avec les 256 premiers cles/valeurs
  struct hashmap_s dict;
  if (0 != hashmap_create(TAILLE_DICT, &dict)) {
    raler(0, "Erreur: creation hashmap\n");
  }

  char tableau_cles[65536][TAILLE_MAX_STRING];
  char tableau_valeurs[65536][TAILLE_MAX_HEXA_STRING];
  int taille_actuelle_dico = 255;

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
  }

  /* Output code for P */
  strcpy(valeur_P, hashmap_get(&dict, cle_P, strlen(cle_P)));
  wb_hex_as_short(fichier_destination, valeur_P);
  printf("Output code for P = %s\n", cle_P);

  hashmap_destroy(&dict);
}

void decompresser_hashmap(FILE* fichier_source, FILE* fichier_destination) {
  // Initialiser le dictionnaire avec les 256 premiers cles/valeurs
  struct hashmap_s dict;
  if (0 != hashmap_create(TAILLE_DICT, &dict)) {
    raler(0, "Erreur: creation hashmap");
  }

  char tableau_cles[65536][TAILLE_MAX_HEXA_STRING];
  char tableau_valeurs[65536][TAILLE_MAX_STRING];
  int taille_actuelle_dico = 255;

  for (int i = 0; i <= 255; i++) {
    sprintf(tableau_cles[i], "%08X", i);
    char* charstr = char2str(i);
    strcpy(tableau_valeurs[i], charstr);
    free(charstr);

    if (0 != hashmap_put(&dict, tableau_cles[i], strlen(tableau_cles[i]),
                         tableau_valeurs[i])) {
      fprintf(stderr, "☢️  Erreur hashmap put\n");
    }
  }

  // Variables utiles pour l'algorithme
  char cle_Old[TAILLE_MAX_HEXA_STRING] = "";
  char* valeur_Old;
  char cle_New[TAILLE_MAX_HEXA_STRING] = "";
  char* valeur_New;
  char valeur_S[TAILLE_MAX_HEXA_STRING] = "";
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

    /* NEW = Next input code */
    char* new = rb_next_short_as_hex(fichier_source);
    // Fin
    if (new == NULL) {
      hashmap_destroy(&dict);
      return;
    }

    strcpy(cle_New, new);
    free(new);
    valeur_New = hashmap_get(&dict, cle_New, strlen(cle_New));
    printf("NEW = next input code : %s : %s\n", cle_New, valeur_New);
    if (hashmap_get(&dict, cle_New, strlen(cle_New)) == NULL) {
      printf("🛂  NEW is not in the string table\n");
      /* S = translation of OLD + C */
      strcpy(valeur_S, concat(valeur_Old, char2str(char_C)));
      printf("S = translation of OLD + C : %s\n", valeur_S);
    } else {
      printf("🛂  NEW is in the string table\n");
      /* S = translation of NEW */
      valeur_New = hashmap_get(&dict, cle_New, strlen(cle_New));
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

    printf("OLD + C  (%s : %s), ajouter a la table a l'indice %d\n",
           tableau_cles[taille_actuelle_dico],
           tableau_valeurs[taille_actuelle_dico], taille_actuelle_dico);

    /* OLD = New */
    strcpy(cle_Old, cle_New);
    valeur_Old = hashmap_get(&dict, cle_Old, strlen(cle_Old));
    printf("OLD = NEW : %s => %s\n", cle_Old, valeur_Old);
  }

  hashmap_destroy(&dict);
}

void compresser_liste(ListeNoeud dict, FILE* fichier_source,
                      FILE* fichier_destination) {
  // Variables utiles pour l'algorithme

  int taille_actuelle_dico = 255;
  char cle_P[TAILLE_MAX_STRING] = "";
  char lecture_char_C;
  char cle_C[TAILLE_MAX_STRING] = "";
  char cle_P_plus_C[TAILLE_MAX_STRING] = "";
  char valeur_P[TAILLE_MAX_STRING] = "";
  char valeur_P_plus_C[TAILLE_MAX_STRING] = "";

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
    if ((strcmp(recuperer_liste(dict, cle_P_plus_C), "NULL")) != 0) {
      strcpy(valeur_P_plus_C, recuperer_liste(dict, cle_P_plus_C));
      printf("P+C = %s is in the string table : %s \n", cle_P_plus_C,
             valeur_P_plus_C);

      /* P = P + C */
      strcpy(cle_P, concatstr);

      /* Else */
    } else {
      printf("P+C = %s is not in the string table\n", cle_P_plus_C);

      /* Output the code for P */
      strcpy(valeur_P, recuperer_liste(dict, cle_P));
      wb_hex_as_short(fichier_destination, valeur_P);
      printf("Output the code for P : %s : %s\n", cle_P, valeur_P);

      /* Add P+C to the string table */
      taille_actuelle_dico++;
      sprintf(valeur_P_plus_C, "%08X", taille_actuelle_dico);
      printf("Add P+C (%s : %s) to the string table at index (%d)\n",
             cle_P_plus_C, valeur_P_plus_C, taille_actuelle_dico);
      inserer_liste(dict, cle_P_plus_C, valeur_P_plus_C);

      /* P = C */
      strcpy(cle_P, cle_C);
    }
    free(concatstr);
  }

  /* Output code for P */
  strcpy(valeur_P, recuperer_liste(dict, cle_P));
  wb_hex_as_short(fichier_destination, valeur_P);
  printf("Output code for P = %s\n", cle_P);
}

void decompresser_liste(ListeNoeud dict, FILE* fichier_source,
                        FILE* fichier_destination) {
  // Variables utiles pour l'algorithme

  int taille_actuelle_dico = 255;
  char cle_Old[TAILLE_MAX_HEXA_STRING] = "";
  char valeur_Old[TAILLE_MAX_HEXA_STRING] = "";
  char cle_New[TAILLE_MAX_HEXA_STRING] = "";
  char valeur_New[TAILLE_MAX_HEXA_STRING] = "";
  char valeur_S[TAILLE_MAX_HEXA_STRING] = "";
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

    /* NEW = Next input code */
    char* new = rb_next_short_as_hex(fichier_source);
    if (new == NULL) return;  // Fin
    strcpy(cle_New, new);
    free(new);
    printf("NEW = next input code : %s : %s\n", cle_New,
           recuperer_liste(dict, cle_New));
    if (strcmp(recuperer_liste(dict, cle_New), "NULL") == 0) {
      printf("🛂  NEW is not in the string table\n");
      /* S = translation of OLD + C */
      charstr = char2str(char_C);
      charstr = concat(valeur_Old, charstr);
      strcpy(valeur_S, charstr);
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
    charstr = char2str(char_C);
    charstr = concat(valeur_Old, charstr);
    inserer_liste(dict, cle_old_c, charstr);

    printf("OLD + C  (%s : %s), ajouter a la table a l'indice %d\n", cle_old_c,
           charstr, taille_actuelle_dico);
    free(charstr);

    /* OLD = New */
    strcpy(cle_Old, cle_New);
    strcpy(valeur_Old, recuperer_liste(dict, cle_Old));
    printf("OLD = NEW : %s => %s\n", cle_Old, valeur_Old);
  }
}

void lzw_compresser(const char* fichier, int mode) {
  // Ouvrir le fichier source
  FILE* fichier_source = fopen(fichier, "r");

  // Creer le nom du fichier destination
  int src_name_size = strlen(fichier);
  char nom_fichier_destination[src_name_size];
  strcpy(nom_fichier_destination, fichier);
  for (int i = src_name_size - 1; i >= src_name_size - 4; i--) {
    remove_char(nom_fichier_destination, i);
  }

  // Ouvrir le fichier destination
  char* nouveau_nom = concat(nom_fichier_destination, "2.lzw");
  FILE* fichier_destination = fopen(nouveau_nom, "wb");
  free(nouveau_nom);

  // Verifier descripteurs de fichier
  if (fichier_source == NULL || fichier_destination == NULL) {
    raler(0, "Erreur: ouverture de fichiers");
  }

  switch (mode) {
    case 1: {
      // Initialiser le dictionnaire avec les 256 premiers cles/valeurs
      ListeNoeud dict_l = NULL;

      for (int i = 0; i < 256; i++) {
        char cle[TAILLE_MAX_STRING] = "";
        char valeur[TAILLE_MAX_HEXA_STRING] = "";
        sprintf(valeur, "%08X", i);
        char* charstr = char2str(i);
        strcpy(cle, charstr);
        free(charstr);
        dict_l = inserer_liste(dict_l, cle, valeur);
      }

      // Compresser en utilisation la structure liste-chainee
      compresser_liste(dict_l, fichier_source, fichier_destination);

      // Liberer l'espace memoire utilisee
      liberer_liste(dict_l);
      break;
    }
    case 2: {
      // Initialiser le dictionnaire avec les 256 premiers cles/valeurs
      TrieNoeud dict_t = creer_noeud();

      for (int i = 0; i < 256; i++) {
        char cle[TAILLE_MAX_STRING] = "";
        char valeur[TAILLE_MAX_HEXA_STRING] = "";
        char* charstr = char2str(i);
        strcpy(cle, charstr);
        free(charstr);
        sprintf(valeur, "%08X", i);
        inserer_trie(dict_t, cle, valeur);
      }

      // Compresser en utilisation la structure trie
      compresser_trie(dict_t, fichier_source, fichier_destination);

      // Liberer l'espace memoire utilisee
      liberer_trie(dict_t);
      break;
    }
    case 3:
      compresser_hashmap(fichier_source, fichier_destination);
    default:
      break;
  }

  fclose(fichier_source);
  fclose(fichier_destination);

  return;
}

void lzw_decompresser(const char* fichier, int mode) {
  // Ouvrir le fichier source
  FILE* fichier_source = fopen(fichier, "rb");

  // Creer le nom du fichier destination
  int src_name_size = strlen(fichier);
  char nom_fichier_destination[src_name_size];
  strcpy(nom_fichier_destination, fichier);
  for (int i = src_name_size - 1; i >= src_name_size - 4; i--) {
    remove_char(nom_fichier_destination, i);
  }
  char* charstr = concat(nom_fichier_destination, ".txt");
  strcpy(nom_fichier_destination, charstr);
  free(charstr);

  // Ouvrir le fichier destination
  FILE* fichier_destination = fopen(nom_fichier_destination, "w");

  // Verifier descripteurs de fichier
  if (fichier_source == NULL || fichier_destination == NULL) {
    raler(0, "Erreur: ouverture des fichiers");
  }

  switch (mode) {
    case 1: {
      // Initialiser le dictionnaire avec les 256 premiers cles/valeurs
      ListeNoeud dict_l = NULL;

      for (int i = 0; i < 256; i++) {
        char cle[TAILLE_MAX_HEXA_STRING] = "";
        char valeur[TAILLE_MAX_STRING] = "";
        sprintf(cle, "%08X", i);
        char* charstr = char2str(i);
        strcpy(valeur, charstr);
        free(charstr);
        dict_l = inserer_liste(dict_l, cle, valeur);
      }

      // Decompresser en utilisation la structure liste-chainee
      decompresser_liste(dict_l, fichier_source, fichier_destination);

      // Liberer l'espace memoire utilisee
      liberer_liste(dict_l);
      break;
    }
    case 2:
      printf("Initialisation Trie\n");
      // Initialiser le dictionnaire avec les 256 premiers cles/valeurs
      TrieNoeud dict_t = creer_noeud();

      for (int i = 0; i < 256; i++) {
        char cle[TAILLE_MAX_HEXA_STRING] = "";
        char valeur[TAILLE_MAX_STRING] = "";
        sprintf(cle, "%08X", i);
        strcpy(valeur, char2str(i));
        inserer_trie(dict_t, cle, valeur);
      }

      // Decompresser en utilisation la structure trie
      decompresser_trie(dict_t, fichier_source, fichier_destination);

      // Liberer l'espace memoire utilisee
      liberer_trie(dict_t);
      break;
    case 3:
      printf("Initialisation Hashmap\n");
      decompresser_hashmap(fichier_source, fichier_destination);
    default:
      break;
  }

  fclose(fichier_source);
  fclose(fichier_destination);
  return;
}

// Main
int main(int argc, char* argv[]) {
  bool s_flag = false;
  int c;

  // On verifie les arguments
  while ((c = getopt(argc, argv, "s:")) != -1) switch (c) {
      case 's':
        s_flag = true;
        break;
      case '?':
        if (optopt == 's') raler(0, "Option -s requiert une option\n");
        if (isprint(optopt))
          raler(0, "Option '-%c' inconnue.\n", optopt);
        else
          raler(0, "Charactere d'option `\\x%x' inconnue.\n", optopt);
        return 1;
      default:
        abort();
    }

  if ((s_flag && argc != 4) || (!s_flag && argc != 2)) {
    raler(0,
          "Nombre d'arguments incorrects. Utilisation: /bin/lzw "
          "[options] <chemin_fichier>\n");
  }

  // Cas 1: Il y a des options
  int mode;
  if (s_flag) {
    if (strcmp(argv[2], "liste-chainee") == 0) {
      mode = 1;
    } else if (strcmp(argv[2], "trie") == 0) {
      mode = 2;
    } else if (strcmp(argv[2], "hashmap") == 0) {
      mode = 3;
    } else {
      raler(0,
            "Mode de compression inconnu. Veuillez choisir entre "
            "\"liste-chainee\", \"trie\" ou \"hashmap\" \n");
    }
    // Cas 2: il n'y a pas d'options donc on chosit le mode par defaut
  } else {
    mode = 0;
  }

  // Si le fichier finit par ".txt", on compresse
  if (strcmp(extension_fichier(argv[3]), "txt") == 0) {
    lzw_compresser(argv[3], mode);
    // Si fichier finit par ".lzw", on decompresse
  } else if (strcmp(extension_fichier(argv[3]), "lzw") == 0) {
    lzw_decompresser(argv[3], mode);
    // Sinon on rale
  } else {
    raler(0,
          "Veuillez chosir un fichier avec une extension \".lzw\" ou "
          "\".txt\"\n");
  }

  return 0;
}