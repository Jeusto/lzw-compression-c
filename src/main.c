#include "../include/main.h"

#define TAILLE_DICT 65536

void decompresser_liste(FILE* fichier_source, FILE* fichier_destination) {
  // Initialiser la table avec des des chaines de 1 caractere
  ListeNoeud dict = NULL;

  for (int i = 0; i < 256; i++) {
    char cle[9] = "";
    char valeur[9] = "";
    sprintf(cle, "%08x", i);
    strcpy(valeur, char2str(i));
    dict = inserer_liste(dict, cle, valeur);
  }

  // Variables utiles pour l'algorithme
  char* element;
  int taille_actuelle_dico = 255;
  char lecture_Old[256] = "";
  char Old_Translation[256] = "";
  char lecture_New[256] = "";
  char New_Translation[256] = "";
  char S_Translation[256] = "";
  char C_Char;

  // Algorithme de compression

  /* OLD = first input code */
  strcpy(lecture_Old, rb_next_int_as_hex(fichier_source));
  strcpy(Old_Translation, recuperer_liste(dict, lecture_Old));
  printf("OLD = first input code : %s => %s\n", lecture_Old, Old_Translation);

  /* Output translation of OLD */
  printf("Output translation of OLD => %s\n", Old_Translation);
  fprintf(fichier_destination, "%s", Old_Translation);

  /* While not end of input stream */
  int iteration = 0;
  while (!feof(fichier_source)) {
    printf("While not end of input stream\n");
    printf("🏅 Iteration = %d\n", iteration++);

    /* NEW = Next input code */
    strcpy(lecture_New, rb_next_short_as_hex(fichier_source));
    if (strcmp(recuperer_liste(dict, lecture_New), "NULL") == 0) {
      break;
    } else {
      strcpy(New_Translation, recuperer_liste(dict, lecture_New));
    }
    printf("NEW = next input code : %s => %s\n", lecture_New, New_Translation);

    /* If NEW is not in the string table */
    printf("If NEW is not in the string table\n");
    if (New_Translation == NULL) {
      printf("NEW is not in the string table\n");
      /* S = translation of OLD */
      strcpy(S_Translation, Old_Translation);
      /* S = S + C */
      strcpy(S_Translation, concat(S_Translation, char2str(C_Char)));
    }
    /* Else */
    else {
      printf("NEW is in the table\n");
      /* S = translation of NEW */
      strcpy(S_Translation, New_Translation);
      printf("S = translation of NEW : . => %s\n", S_Translation);
    }

    /* Output S */
    printf("OUTPUT S : . => %s\n", S_Translation);
    fprintf(fichier_destination, "%s", S_Translation);

    /* C = first character of S */
    C_Char = S_Translation[0];
    printf("C = premier char de S : %c\n", C_Char);

    /* OLD + C to the string table */
    taille_actuelle_dico++;
    printf("OLD + C to the table : %c\n", C_Char);

    char cle_old_c[256] = "";
    sprintf(cle_old_c, "%08x", taille_actuelle_dico);
    inserer_liste(dict, cle_old_c, concat(Old_Translation, char2str(C_Char)));

    /* OLD = New */
    strcpy(lecture_Old, lecture_New);
    printf("OLD = NEW : %s\n", lecture_Old);
  }
}

void compresser_liste(FILE* fichier_source, FILE* fichier_destination) {
  // Initialiser la table avec des des chaines de 1 caractere
  ListeNoeud dict = NULL;

  for (int i = 0; i < 256; i++) {
    char cle[9] = "";
    char valeur[9] = "";
    strcpy(cle, char2str(i));
    sprintf(valeur, "%08x", i);
    dict = inserer_liste(dict, cle, valeur);
  }

  // Variables utiles pour l'algorithme
  char* element;
  int taille_actuelle_dico = 255;
  char cle_P[256] = "";
  char lecture_char_C;
  char cle_C[256] = "";
  char cle_P_plus_C[256] = "";
  char valeur_P[256] = "";
  char valeur_P_plus_C[256] = "";

  // Algorithme de compression

  /* P = first input character */
  strcpy(cle_P, char2str(fgetc(fichier_source)));
  strcpy(valeur_P, recuperer_liste(dict, cle_P));

  /* While not end of input stream*/
  /* C = next input character */
  while ((lecture_char_C = fgetc(fichier_source)) != EOF) {
    strcpy(cle_C, char2str(lecture_char_C));

    /* If P+C is in the string table */
    strcpy(cle_P_plus_C, concat(cle_P, cle_C));
    strcpy(valeur_P_plus_C, recuperer_liste(dict, cle_P_plus_C));
    if (!(strcmp(valeur_P_plus_C, "NULL") == 0)) {
      printf("P+C = %s is in the string table : %s \n", cle_P_plus_C,
             valeur_P_plus_C);

      /* P = P + C */
      strcpy(cle_P, concat(cle_P, cle_C));

      /* Else */
    } else {
      printf("P+C = %s is not in the string table\n", cle_P_plus_C);

      /* Output the code for P */
      strcpy(valeur_P, recuperer_liste(dict, cle_P));
      wb_hex_as_int(fichier_destination, valeur_P);
      printf("Output the code for P : %s : %s\n", cle_P, valeur_P);

      /* Add P+C to the string table */
      taille_actuelle_dico++;
      sprintf(valeur_P_plus_C, "%08x", taille_actuelle_dico);
      printf("Add P+C (%s : %s) to the string table at index (%d)\n",
             cle_P_plus_C, valeur_P_plus_C, taille_actuelle_dico);
      inserer_liste(dict, cle_P_plus_C, valeur_P_plus_C);

      strcpy(cle_P, cle_C);
    }
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
  FILE* fichier_destination =
      fopen(concat(nom_fichier_destination, "2.lzw"), "wb");

  // Verifier descripteurs de fichier
  if (fichier_source == NULL || fichier_destination == NULL) {
    fprintf(stderr, "☢️  Erreur lors de l'ouverture d'un des fichiers\n");
    exit(EXIT_FAILURE);
  }

  switch (mode) {
    case 1:
      printf("Initialisation Liste\n");
      compresser_liste(fichier_source, fichier_destination);
      break;
    case 2:
      printf("Initialisation TrieNoeud\n");
      break;
    case 3:
      printf("Initialisation Hashmap\n");
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
      decompresser_liste(fichier_source, fichier_destination);
      break;
    case 2:
      printf("Initialisation Trie\n");
      break;
    case 3:
      printf("Initialisation Hashmap\n");
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