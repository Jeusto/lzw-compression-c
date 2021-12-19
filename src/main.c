
#include <argp.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "../include/lzw.h"

#define TAILLE_DICT 65536
#define TAILLE_MAX_HEXA_STRING 9
#define TAILLE_MAX_STRING 64

// Fonction appelé lorsque l'utilisateur veut compresser un fichier
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

  // Creer et ouvrir le fichier destination
  char* nouveau_nom = concat(nom_fichier_destination, ".lzw");
  FILE* fichier_destination = fopen(nouveau_nom, "wb");
  free(nouveau_nom);

  // Verifier descripteurs de fichier
  if (fichier_source == NULL || fichier_destination == NULL) {
    raler(0, "erreur ouverture: %s", fichier);
  }

  // Si le fichier source est vide, on quitte tout de suite apres avoir cree
  fseek(fichier_source, 0L, SEEK_END);
  if (ftell(fichier_source) == 0) {
    fclose(fichier_source);
    fclose(fichier_destination);
    return;
  }
  fseek(fichier_source, 0L, SEEK_SET);

  // On appelle la bonne fonction en fonction du mode
  switch (mode) {
    case 1: {
      // Initialiser le dictionnaire avec les 256 premiers cles/valeurs
      ListeNoeud dict_l = initialiser_liste("compression");

      // Compresser en utilisation la structure liste-chainee
      compresser_liste(dict_l, fichier_source, fichier_destination);
      break;
    }
    case 2: {
      // Initialiser le dictionnaire avec les 256 premiers cles/valeurs
      TrieNoeud dict_t = initialiser_trie("compression");

      // Compresser en utilisation la structure trie
      compresser_trie(dict_t, fichier_source, fichier_destination);
      break;
    }
    case 3: {
      // Initialiser le dictionnaire avec les 256 premiers cles/valeurs
      char** tableau_cles = malloc(TAILLE_DICT * TAILLE_MAX_STRING);
      for (int i = 0; i < TAILLE_DICT; i++) {
        tableau_cles[i] = malloc(TAILLE_MAX_STRING);
      }
      char** tableau_valeurs = malloc(TAILLE_DICT * TAILLE_MAX_HEXA_STRING);
      for (int i = 0; i < TAILLE_DICT; i++) {
        tableau_valeurs[i] = malloc(TAILLE_MAX_HEXA_STRING);
      }
      struct hashmap_s dict_h =
          initialiser_hashmap("compression", tableau_cles, tableau_valeurs);

      compresser_hashmap(fichier_source, fichier_destination, dict_h,
                         tableau_cles, tableau_valeurs);
    }
    default:
      break;
  }

  fclose(fichier_source);
  fclose(fichier_destination);

  return;
}

// Fonction appelé lorsque l'utilisateur veut compresser un fichier
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
    raler(0, "erreur ouverture: %s", fichier);
  }

  // Si le fichier source est vide, on quitte tout de suite
  fseek(fichier_source, 0L, SEEK_END);
  if (ftell(fichier_source) == 0) {
    fclose(fichier_source);
    fclose(fichier_destination);
    return;
  }
  fseek(fichier_source, 0L, SEEK_SET);

  switch (mode) {
    case 1: {
      // Initialiser le dictionnaire avec les 256 premiers cles/valeurs
      ListeNoeud dict_l = initialiser_liste("decompression");

      // Decompresser en utilisation la structure liste-chainee
      decompresser_liste(dict_l, fichier_source, fichier_destination);
      break;
    }
    case 2: {
      // Initialiser le dictionnaire avec les 256 premiers cles/valeurs
      TrieNoeud dict_t = initialiser_trie("decompression");

      // Decompresser en utilisation la structure trie
      decompresser_trie(dict_t, fichier_source, fichier_destination);
      break;
    }
    case 3: {
      // Initialiser le dictionnaire avec les 256 premiers cles/valeurs
      char** tableau_cles = malloc(TAILLE_DICT * TAILLE_MAX_HEXA_STRING);
      for (int i = 0; i < TAILLE_DICT; i++) {
        tableau_cles[i] = malloc(TAILLE_MAX_HEXA_STRING);
      }
      char** tableau_valeurs = malloc(TAILLE_DICT * TAILLE_MAX_STRING);
      for (int i = 0; i < TAILLE_DICT; i++) {
        tableau_valeurs[i] = malloc(TAILLE_MAX_STRING);
      }
      struct hashmap_s dict_h =
          initialiser_hashmap("decompression", tableau_cles, tableau_valeurs);

      decompresser_hashmap(fichier_source, fichier_destination, dict_h,
                           tableau_cles, tableau_valeurs);
    }
    default:
      break;
  }

  fclose(fichier_source);
  fclose(fichier_destination);
  return;
}

int main(int argc, char* argv[]) {
  bool s_flag = false;
  int c;

  // On vérifie les arguments
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
    mode = 3;
  }

  // Si le fichier finit par ".txt", on compresse
  if (strcmp(extension_fichier(argv[argc - 1]), "txt") == 0) {
    // Print all
    lzw_compresser(argv[argc - 1], mode);
    // Si fichier finit par ".lzw", on decompresse
  } else if (strcmp(extension_fichier(argv[argc - 1]), "lzw") == 0) {
    lzw_decompresser(argv[argc - 1], mode);
    // Sinon on rale
  } else {
    raler(0,
          "Veuillez chosir un fichier avec une extension \".lzw\" ou "
          "\".txt\"\n");
  }

  return 0;
}