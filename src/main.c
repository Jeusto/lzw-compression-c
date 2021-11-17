#include "../include/main-lzw.h"

#define TAILLE_DICT 65536

void lzw_compresser(const char* fichier, const char* mode) {
  printf("📌 Compression en utilisant structure de donnees \"%s\"\n", mode);

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
      fopen(concat(nom_fichier_destination, ".lzw"), "wb");

  // Verifier descripteurs de fichier
  if (fichier_source == NULL || fichier_destination == NULL) {
    fprintf(stderr, "☢️  Erreur lors de l'ouverture d'un des fichiers\n");
    exit(EXIT_FAILURE);
  }

  // Mettre les 256 premiers symboles dans la table d'encodage
  struct hashmap_s hashmap;
  if (0 != hashmap_create(TAILLE_DICT, &hashmap)) {
    fprintf(stderr, "Erreur create\n");
  }

  char* element;
  char tableau_cles[65536][9];
  char tableau_valeurs[65536][9];
  int taille_actuelle_dico = 255;

  for (int i = 0; i <= 255; i++) {
    strcpy(tableau_cles[i], char2str(i));
    sprintf(tableau_valeurs[i], "%08x", i);

    if (0 != hashmap_put(&hashmap, tableau_cles[i], strlen(tableau_cles[i]),
                         tableau_valeurs[i])) {
      fprintf(stderr, "☢️  Erreur hashmap put\n");
    }
  }

  // // Print all values in hashmap
  // for (int i = 0; i <= 255; i++) {
  //   element = hashmap_get(&hashmap, tableau_cles[i],
  //   strlen(tableau_cles[i])); printf("cle = %s / valeur = %s\n",
  //   tableau_cles[i], element);
  // }

  // // Test hashmap
  // char* test = hashmap_get(&hashmap, "A", strlen("B"));
  // if (test) {
  //   printf("Yes : \"%s\"\n\n", test);
  // } else {
  //   printf("Pas de valeur\n\n");
  // }

  // A chaque etape : verifier si les symboles sont dans la table
  // Si oui, on l'encode avec son index et on cree un nouveau symbole
  // en concatenant avec le symbole suivant
  char lecture_P[256];
  char lecture_char_C;
  char lecture_C[256];
  char lecture_P_plus_C[256];
  char* valeur_P;
  char* valeur_P_plus_C;

  /* P = first input character */
  strcpy(lecture_P, char2str(fgetc(fichier_source)));

  /* While not end of input stream*/
  /* C = next input character */
  while ((lecture_char_C = fgetc(fichier_source)) != EOF) {
    strcpy(lecture_C, char2str(lecture_char_C));
    strcpy(lecture_P_plus_C, concat(lecture_P, lecture_C));

    printf("\nTaille = %u / cle P = %s / cle C = %s / cle P+C = %s\n",
           hashmap_num_entries(&hashmap), lecture_P, lecture_C,
           lecture_P_plus_C);

    valeur_P = hashmap_get(&hashmap, lecture_P, strlen(lecture_P));
    valeur_P_plus_C =
        hashmap_get(&hashmap, lecture_P_plus_C, strlen(lecture_P_plus_C));
    printf("Taille = %u / valeur P = %s / valeur P+C = %s\n",
           hashmap_num_entries(&hashmap), valeur_P, valeur_P_plus_C);

    /* If P+C is in the string table */
    if (valeur_P_plus_C) {
      /* P = P + C */
      strcpy(lecture_P, concat(lecture_P, lecture_C));
      printf("P+C est dans la table\n");

      /* Else */
    } else {
      /* Output the code for P */
      wb_hex_as_int(fichier_destination, valeur_P);
      printf("P+C n'est pas dans la table\n");
      printf("J'output P = %s\n", lecture_P);

      /* Add P+C to the string table */
      taille_actuelle_dico++;

      strcpy(tableau_cles[taille_actuelle_dico], lecture_P_plus_C);
      sprintf(tableau_valeurs[taille_actuelle_dico], "%08x",
              taille_actuelle_dico);

      hashmap_put(&hashmap, tableau_cles[taille_actuelle_dico],
                  strlen(tableau_cles[taille_actuelle_dico]),
                  tableau_valeurs[taille_actuelle_dico]);
      printf("J'ajoute P+C = %s a la table indice %d\n", lecture_P_plus_C,
             taille_actuelle_dico);

      strcpy(lecture_P, lecture_C);
    }
    /* End if */
  }
  /* End while */

  /* Output code for P */
  wb_hex_as_int(fichier_destination, valeur_P);
  printf("J'output P = %s\n", lecture_P);

  fclose(fichier_source);
  fclose(fichier_destination);

  return;
}

void lzw_decompresser(char* fichier, char* mode) {
  printf("📌 Decompression en utilisant structure de donnees \"%s\"\n", mode);

  // Ouvrir le fichier source
  FILE* fichier_source = fopen(fichier, "rb");

  // Creer le nom du fichier destination
  int src_name_size = strlen(fichier);
  char nom_fichier_destination[src_name_size];
  strcpy(nom_fichier_destination, fichier);
  for (int i = src_name_size - 1; i >= src_name_size - 4; i--) {
    remove_char(nom_fichier_destination, i);
  }

  // Ouvrir le fichier destination
  FILE* fichier_destination =
      fopen(concat(nom_fichier_destination, ".txt"), "w");

  // Verifier descripteurs de fichier
  if (fichier_source == NULL || fichier_destination == NULL) {
    fprintf(stderr, "☢️  Erreur lors de l'ouverture d'un des fichiers\n");
    exit(EXIT_FAILURE);
  }

  // Mettre les 256 premiers symboles dans la table d'encodage
  struct hashmap_s hashmap;
  if (0 != hashmap_create(TAILLE_DICT, &hashmap)) {
    fprintf(stderr, "Erreur create\n");
  }

  char* element;
  char tableau_cles[65536][9];
  char tableau_valeurs[65536][9];
  int taille_actuelle_dico = 255;

  for (int i = 0; i <= 255; i++) {
    sprintf(tableau_cles[i], "%08x", i);
    strcpy(tableau_valeurs[i], char2str(i));

    if (0 != hashmap_put(&hashmap, tableau_cles[i], strlen(tableau_cles[i]),
                         tableau_valeurs[i])) {
      fprintf(stderr, "☢️  Erreur hashmap put\n");
    }
  }

  // // Algorithme de decompression LZW
  char lecture_Old[256];
  char* Old_Translation;
  char lecture_New[256];
  char* New_Translation;
  char* S_Translation;
  char C_Char;

  /* OLD = first input code */
  strcpy(lecture_Old, rb_next_int_as_hex(fichier_source));
  Old_Translation = hashmap_get(&hashmap, lecture_Old, strlen(lecture_Old));

  /* Output translation of OLD */
  printf("J'output Old = %s\n", Old_Translation);
  fprintf(fichier_destination, "%s", Old_Translation);

  /* While not end of input stream */
  /* New = next input code */
  strcpy(lecture_New, "");

  while (lecture_New != EOF) {
    /* New = Next input code */
    strcpy(lecture_New, rb_next_int_as_hex(fichier_source));
    New_Translation = hashmap_get(&hashmap, lecture_New, strlen(lecture_New));

    /* If New is not in the string table */
    if (New_Translation == NULL) {
      /* S = translation of OLD */
      strcpy(S_Translation, Old_Translation);
      /* S = S + C */
      strcpy(S_Translation, concat(S_Translation, char2str(C_Char)));
    }
    /* Else */
    else {
      strcpy(S_Translation, New_Translation);
    }
    /* Output S */
    fprintf(fichier_destination, "%s", S_Translation);

    /* C = first character of S */
    C_Char = S_Translation[0];

    /* OLD + C to the string table */
    taille_actuelle_dico++;

    sprintf(tableau_cles[taille_actuelle_dico], "%08x", taille_actuelle_dico);
    strcpy(tableau_valeurs[taille_actuelle_dico],
           concat(Old_Translation, C_Char));

    hashmap_put(&hashmap, tableau_cles[taille_actuelle_dico],
                strlen(tableau_cles[taille_actuelle_dico]),
                tableau_valeurs[taille_actuelle_dico]);

    /* OLD = New */
    strcpy(lecture_Old, lecture_New);
  }

  fclose(fichier_source);
  fclose(fichier_destination);

  return;
}

bool fichier_existe(char* fichier) {
  if (access(fichier, F_OK) != 0) {
    return false;
  }
  return true;
}

// void* initialiser_dictionnaire(int taille_dictionnaire, void *structure) {}
// void exit_propre(char* message) {}

const char* extension_fichier(const char* fichier) {
  const char* point = strrchr(fichier, '.');
  if (!point || point == fichier) return "";
  return point + 1;
}

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
    const char* structure_a_utiliser = l_flag   ? "liste_chainee"
                                       : t_flag ? "trie"
                                                : "hashmap";

    if (strcmp(extension_fichier(argv[2]), "txt") == 0) {
      lzw_compresser(argv[2], structure_a_utiliser);
    };

    if (strcmp(extension_fichier(argv[2]), "lzw") == 0) {
      lzw_decompresser(argv[2], structure_a_utiliser);
    }
  }

  // Cas 2: Il n'y a pas d'options
  else if (!options_existe && argc == 2) {
    if (strcmp(extension_fichier(argv[1]), "txt") == 0) {
      lzw_compresser(argv[1], "optimale");
    } else if (strcmp(extension_fichier(argv[1]), "lzw") == 0) {
      lzw_decompresser(argv[1], "optimale");
    } else {
      fprintf(stderr,
              "Nombre d'arguments incorrects. Utilisation: /bin/lzw "
              "[options] <chemin_fichier>\n");
      exit(EXIT_FAILURE);
    }
  }

  return 0;
}