#include "../include/main.h"

#define TAILLE_DICT 65536
#define TAILLE_MAX_HEXA_STRING 32
#define TAILLE_MAX_STRING 64

void compresser_trie(TrieNoeud dict, FILE* fichier_source,
                     FILE* fichier_destination) {
  // Variables utiles pour l'algorithme
  char* element;
  int taille_actuelle_dico = 255;
  char cle_P[TAILLE_MAX_STRING] = "";
  char lecture_char_C;
  char cle_C[TAILLE_MAX_STRING] = "";
  char cle_P_plus_C[TAILLE_MAX_STRING] = "";
  char valeur_P[TAILLE_MAX_STRING] = "";
  char valeur_P_plus_C[TAILLE_MAX_STRING] = "";

  // Algorithme de compression

  /* P = first input character */
  strcpy(cle_P, char2str(fgetc(fichier_source)));
  strcpy(valeur_P, recuperer_trie(dict, cle_P));

  /* While not end of input stream*/
  /* C = next input character */
  while ((lecture_char_C = fgetc(fichier_source)) != EOF) {
    strcpy(cle_C, char2str(lecture_char_C));

    /* If P+C is in the string table */
    strcpy(cle_P_plus_C, concat(cle_P, cle_C));

    if ((strcmp(recuperer_trie(dict, cle_P_plus_C), "NULL")) != 0) {
      strcpy(valeur_P_plus_C, recuperer_trie(dict, cle_P_plus_C));
      printf("P+C = %s is in the string table : %s \n", cle_P_plus_C,
             valeur_P_plus_C);

      /* P = P + C */
      strcpy(cle_P, concat(cle_P, cle_C));

      /* Else */
    } else {
      printf("P+C = %s is not in the string table\n", cle_P_plus_C);

      /* Output the code for P */
      strcpy(valeur_P, recuperer_trie(dict, cle_P));
      wb_hex_as_int(fichier_destination, valeur_P);
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
  wb_hex_as_int(fichier_destination, valeur_P);
  printf("Output code for P = %s\n", cle_P);
}

void decompresser_trie(TrieNoeud dict, FILE* fichier_source,
                       FILE* fichier_destination) {
  // Variables utiles pour l'algorithme
  char* element;
  int taille_actuelle_dico = 255;
  char cle_Old[TAILLE_MAX_HEXA_STRING] = "";
  char valeur_Old[TAILLE_MAX_HEXA_STRING] = "";
  char cle_New[TAILLE_MAX_HEXA_STRING] = "";
  char valeur_New[TAILLE_MAX_HEXA_STRING] = "";
  char valeur_S[TAILLE_MAX_HEXA_STRING] = "";
  char char_C;

  // Algorithme de decompression

  /* OLD = first input code */
  printf("🤬 Debut\n");
  strcpy(cle_Old, rb_next_int_as_hex(fichier_source));
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
    char* new = rb_next_int_as_hex(fichier_source);
    if (new == NULL) exit(1);  // Fin
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
  // Initialiser la table avec des des chaines de 1 caractere
  struct hashmap_s dict;
  if (0 != hashmap_create(TAILLE_DICT, &dict)) {
    fprintf(stderr, "Erreur create\n");
  }

  char* element;
  char tableau_cles[65536][TAILLE_MAX_STRING];
  char tableau_valeurs[65536][TAILLE_MAX_HEXA_STRING];
  int taille_actuelle_dico = 255;

  for (int i = 0; i <= 255; i++) {
    strcpy(tableau_cles[i], char2str(i));

    sprintf(tableau_valeurs[i], "%08X", i);

    if (0 != hashmap_put(&dict, tableau_cles[i], strlen(tableau_cles[i]),
                         tableau_valeurs[i])) {
      fprintf(stderr, "☢️  Erreur hashmap put\n");
    }
  }

  // Variables utiles pour l'algorithme
  char cle_P[TAILLE_MAX_STRING] = "";
  char lecture_char_C;
  char cle_C[TAILLE_MAX_STRING] = "";
  char cle_P_plus_C[TAILLE_MAX_STRING] = "";
  char* valeur_P;
  char* valeur_P_plus_C;

  // Print all values in hashmap
  // for (int i = 0; i <= 255; i++) {
  //   element = hashmap_get(&dict, tableau_cles[i], strlen(tableau_cles[i]));
  //   printf("cle = %s / valeur = %s\n", tableau_cles[i], element);
  // }
  // Algorithme de compression

  /* P = first input character */
  strcpy(cle_P, char2str(fgetc(fichier_source)));
  valeur_P = hashmap_get(&dict, cle_P, strlen(cle_P));

  /* While not end of input stream*/
  /* C = next input character */
  while ((lecture_char_C = fgetc(fichier_source)) != EOF) {
    strcpy(cle_C, char2str(lecture_char_C));

    /* If P+C is in the string table */

    strcpy(cle_P_plus_C, concat(cle_P, cle_C));
    valeur_P_plus_C = hashmap_get(&dict, cle_P_plus_C, strlen(cle_P_plus_C));
    if (valeur_P_plus_C) {
      printf("P+C = %s is in the string table : %s \n", cle_P_plus_C,
             valeur_P_plus_C);

      /* P = P + C */
      strcpy(cle_P, concat(cle_P, cle_C));

      /* Else */
    } else {
      printf("P+C = %s is not in the string table\n", cle_P_plus_C);

      /* Output the code for P */
      valeur_P = hashmap_get(&dict, cle_P, strlen(cle_P));
      wb_hex_as_int(fichier_destination, valeur_P);
      printf("Output the code for P : %s : %s\n", cle_P, valeur_P);

      /* Add P+C to the string table */
      taille_actuelle_dico++;
      sprintf(tableau_valeurs[taille_actuelle_dico], "%08X",
              taille_actuelle_dico);
      strcpy(tableau_cles[taille_actuelle_dico], cle_P_plus_C);
      hashmap_put(&dict, tableau_cles[taille_actuelle_dico],
                  strlen(tableau_cles[taille_actuelle_dico]),
                  tableau_valeurs[taille_actuelle_dico]);
      printf("Add P+C (%s : %s) to the string table at index (%d)\n",
             tableau_cles[taille_actuelle_dico],
             tableau_valeurs[taille_actuelle_dico], taille_actuelle_dico);

      /* P = C */
      strcpy(cle_P, cle_C);
    }
  }

  /* Output code for P */
  strcpy(valeur_P, hashmap_get(&dict, cle_P, strlen(cle_P)));
  wb_hex_as_int(fichier_destination, valeur_P);
  printf("Output code for P = %s\n", cle_P);

  hashmap_destroy(&dict);
}

void decompresser_hashmap(FILE* fichier_source, FILE* fichier_destination) {
  // Initialiser la table avec des des chaines de 1 caractere
  struct hashmap_s dict;
  if (0 != hashmap_create(TAILLE_DICT, &dict)) {
    fprintf(stderr, "Erreur create\n");
  }

  char* element;
  char tableau_cles[65536][TAILLE_MAX_HEXA_STRING];
  char tableau_valeurs[65536][TAILLE_MAX_STRING];
  int taille_actuelle_dico = 255;

  for (int i = 0; i <= 255; i++) {
    sprintf(tableau_cles[i], "%08X", i);
    strcpy(tableau_valeurs[i], char2str(i));

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
  char char_C;

  // Algorithme de decompression

  /* OLD = first input code */
  printf("🤬 Debut\n");
  strcpy(cle_Old, rb_next_int_as_hex(fichier_source));
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
    char* new = rb_next_int_as_hex(fichier_source);
    if (new == NULL) exit(1);  // Fin

    strcpy(cle_New, new);
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
    strcpy(tableau_valeurs[taille_actuelle_dico],
           concat(valeur_Old, char2str(char_C)));

    hashmap_put(&dict, tableau_cles[taille_actuelle_dico],
                strlen(tableau_cles[taille_actuelle_dico]),
                tableau_valeurs[taille_actuelle_dico]);

    printf("OLD + C  (%s : %s), ajouter a la table a l'indice %d\n",
           tableau_cles[taille_actuelle_dico],
           tableau_valeurs[taille_actuelle_dico], taille_actuelle_dico);

    /* OLD = New */
    strcpy(cle_Old, cle_New);
    valeur_Old = hashmap_get(&dict, cle_Old, strlen(cle_Old));
    printf("OLD = NEW : %s => %s\n", cle_Old, valeur_Old);
  }
}

void decompresser_liste(ListeNoeud dict, FILE* fichier_source,
                        FILE* fichier_destination) {
  // Variables utiles pour l'algorithme
  char* element;
  int taille_actuelle_dico = 255;
  char cle_Old[TAILLE_MAX_HEXA_STRING] = "";
  char valeur_Old[TAILLE_MAX_HEXA_STRING] = "";
  char cle_New[TAILLE_MAX_HEXA_STRING] = "";
  char valeur_New[TAILLE_MAX_HEXA_STRING] = "";
  char valeur_S[TAILLE_MAX_HEXA_STRING] = "";
  char char_C;

  // Algorithme de decompression

  /* OLD = first input code */
  printf("🤬 Debut\n");
  strcpy(cle_Old, rb_next_int_as_hex(fichier_source));
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
    char* new = rb_next_int_as_hex(fichier_source);
    if (new == NULL) exit(1);  // Fin
    strcpy(cle_New, new);
    printf("NEW = next input code : %s : %s\n", cle_New,
           recuperer_liste(dict, cle_New));
    if (strcmp(recuperer_liste(dict, cle_New), "NULL") == 0) {
      printf("🛂  NEW is not in the string table\n");
      /* S = translation of OLD + C */
      strcpy(valeur_S, concat(valeur_Old, char2str(char_C)));
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
    inserer_liste(dict, cle_old_c, concat(valeur_Old, char2str(char_C)));

    printf("OLD + C  (%s : %s), ajouter a la table a l'indice %d\n", cle_old_c,
           concat(valeur_Old, char2str(char_C)), taille_actuelle_dico);

    /* OLD = New */
    strcpy(cle_Old, cle_New);
    strcpy(valeur_Old, recuperer_liste(dict, cle_Old));
    printf("OLD = NEW : %s => %s\n", cle_Old, valeur_Old);
  }
}

void compresser_liste(ListeNoeud dict, FILE* fichier_source,
                      FILE* fichier_destination) {
  // Variables utiles pour l'algorithme

  char* element;
  int taille_actuelle_dico = 255;
  char cle_P[TAILLE_MAX_STRING] = "";
  char lecture_char_C;
  char cle_C[TAILLE_MAX_STRING] = "";
  char cle_P_plus_C[TAILLE_MAX_STRING] = "";
  char valeur_P[TAILLE_MAX_STRING] = "";
  char valeur_P_plus_C[TAILLE_MAX_STRING] = "";

  // Algorithme de compression

  /* P = first input character */
  char* concatstr2 = char2str(fgetc(fichier_source));
  strcpy(cle_P, concatstr2);
  strcpy(valeur_P, recuperer_liste(dict, cle_P));
  free(concatstr2);

  /* While not end of input stream*/
  /* C = next input character */
  while ((lecture_char_C = fgetc(fichier_source)) != EOF) {
    char* concatstr2 = char2str(lecture_char_C);
    strcpy(cle_C, concatstr2);
    free(concatstr2);

    /* If P+C is in the string table */
    char* concatstr;
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
      wb_hex_as_int(fichier_destination, valeur_P);
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
  wb_hex_as_int(fichier_destination, valeur_P);
  printf("Output code for P = %s\n", cle_P);
}

void lzw_compresser(const char* fichier, int mode) {
  printf("📌 Compression en utilisant structure de donnees %d\n", mode);

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
    fprintf(stderr, "☢️  Erreur lors de l'ouverture d'un des fichiers\n");
    exit(EXIT_FAILURE);
  }

  switch (mode) {
    case 1:
      printf("Initialisation Liste\n");
      ListeNoeud dict_l = NULL;
      for (int i = 0; i < 256; i++) {
        char cle[TAILLE_MAX_STRING] = "";
        char valeur[TAILLE_MAX_HEXA_STRING] = "";
        char* charstr = char2str(i);
        strcpy(cle, charstr);
        free(charstr);
        sprintf(valeur, "%08X", i);
        dict_l = inserer_liste(dict_l, cle, valeur);
      }
      compresser_liste(dict_l, fichier_source, fichier_destination);
      liberer_liste(dict_l);
      break;
    case 2:
      printf("Initialisation TrieNoeud\n");
      // Initialiser la table avec des des chaines de 1 caractere
      TrieNoeud dict_t = creer_noeud();
      for (int i = 0; i < 256; i++) {
        char cle[TAILLE_MAX_STRING] = "";
        char valeur[TAILLE_MAX_HEXA_STRING] = "";
        strcpy(cle, char2str(i));
        sprintf(valeur, "%08X", i);
        inserer_trie(dict_t, cle, valeur);
      }
      compresser_trie(dict_t, fichier_source, fichier_destination);
      liberer_trie(dict_t);
      break;
    case 3:
      printf("Initialisation Hashmap\n");
      compresser_hashmap(fichier_source, fichier_destination);
    default:
      exit(1);
  }

  fclose(fichier_source);
  fclose(fichier_destination);
  return;
}

void lzw_decompresser(const char* fichier, int mode) {
  printf("📌 Decompression en utilisant structure de donnees %d\n", mode);

  // Ouvrir le fichier source
  FILE* fichier_source = fopen(fichier, "rb");

  // Creer le nom du fichier destination
  int src_name_size = strlen(fichier);
  char nom_fichier_destination[src_name_size];
  strcpy(nom_fichier_destination, fichier);
  for (int i = src_name_size - 1; i >= src_name_size - 4; i--) {
    remove_char(nom_fichier_destination, i);
  }
  strcpy(nom_fichier_destination, concat(nom_fichier_destination, ".txt"));

  // Ouvrir le fichier destination
  FILE* fichier_destination = fopen(nom_fichier_destination, "w");

  // Verifier descripteurs de fichier
  if (fichier_source == NULL || fichier_destination == NULL) {
    fprintf(stderr, "☢️  Erreur lors de l'ouverture d'un des fichiers\n");
    exit(EXIT_FAILURE);
  }

  switch (mode) {
    case 1:
      printf("Initialisation Liste\n");
      // Initialiser la table avec des des chaines de 1 caractere
      ListeNoeud dict_l = NULL;

      for (int i = 0; i < 256; i++) {
        char cle[TAILLE_MAX_HEXA_STRING] = "";
        char valeur[TAILLE_MAX_STRING] = "";
        sprintf(cle, "%08X", i);
        strcpy(valeur, char2str(i));
        dict_l = inserer_liste(dict_l, cle, valeur);
      }
      decompresser_liste(dict_l, fichier_source, fichier_destination);
      liberer_liste(dict_l);
      break;
    case 2:
      printf("Initialisation Trie\n");
      // Initialiser la table avec des des chaines de 1 caractere
      TrieNoeud dict_t = creer_noeud();

      for (int i = 0; i < 256; i++) {
        char cle[TAILLE_MAX_HEXA_STRING] = "";
        char valeur[TAILLE_MAX_STRING] = "";
        sprintf(cle, "%08X", i);
        strcpy(valeur, char2str(i));
        inserer_trie(dict_t, cle, valeur);
      }

      decompresser_trie(dict_t, fichier_source, fichier_destination);
      liberer_trie(dict_t);
      break;
    case 3:
      printf("Initialisation Hashmap\n");
      decompresser_hashmap(fichier_source, fichier_destination);
    default:
      exit(1);
  }

  fclose(fichier_source);
  fclose(fichier_destination);
  return;
}

// Utils
bool fichier_existe(char* fichier) {
  if (access(fichier, F_OK) != 0) {
    return false;
  }
  return true;
}

// Utils
const char* extension_fichier(const char* fichier) {
  const char* point = strrchr(fichier, '.');
  if (!point || point == fichier) return "";
  return point + 1;
}

// Main
int main(int argc, char* argv[]) {
  bool options_existe = false, l_flag = false, t_flag = false, h_flag = false;
  int c;

  // Recuperer et verifier les arguments
  if (argc < 2 || argc > 3) {
    fprintf(stderr,
            "Nombre d'arguments incorrects. Utilisation: /bin/lzw "
            "[options] <chemin_fichier>\n");
    exit(EXIT_FAILURE);
  }

  while ((c = getopt(argc, argv, "lth:")) != -1) switch (c) {
      case 'l':
        l_flag = true;
        options_existe = true;
        break;
      case 't':
        t_flag = true;
        options_existe = true;
        break;
      case 'h':
        h_flag = true;
        options_existe = true;
        break;
      case '?':
        if (isprint(optopt))
          fprintf(stderr, "Option '-%c' inconnue.\n", optopt);
        else
          fprintf(stderr, "Charactere d'option `\\x%x' inconnue.\n", optopt);
        return 1;
      default:
        abort();
    }
  printf("📌 Arguments : l_flag = %d, t_flag = %d, h_flag = %d, argc = %d\n",
         l_flag, t_flag, h_flag, argc);

  // Cas 1: Il y a des options
  if (options_existe && argc == 3) {
    int mode = l_flag ? 1 : t_flag ? 2 : h_flag ? 3 : -1;
    if (mode < -1) {
      exit(1);
    }

    if (strcmp(extension_fichier(argv[2]), "txt") == 0) {
      lzw_compresser(argv[2], mode);
    };

    if (strcmp(extension_fichier(argv[2]), "lzw") == 0) {
      lzw_decompresser(argv[2], mode);
    }
  }

  // Cas 2: Il n'y a pas d'options
  else if (!options_existe && argc == 2) {
    if (strcmp(extension_fichier(argv[1]), "txt") == 0) {
      lzw_compresser(argv[1], 0);
    } else if (strcmp(extension_fichier(argv[1]), "lzw") == 0) {
      lzw_decompresser(argv[1], 0);
    } else {
      fprintf(stderr,
              "Nombre d'arguments incorrects. Utilisation: /bin/lzw "
              "[options] <chemin_fichier>\n");
      exit(EXIT_FAILURE);
    }
  }

  return 0;
}