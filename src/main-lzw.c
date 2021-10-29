#include "../include/main-lzw.h"

void lzw_compresser(char* fichier, char* mode) {
  printf("compresser\n");
  return;
}

void lzw_decompresser() {
  printf("decompresser\n");
  return;
}

const char* extension_fichier(const char* fichier) {
  const char* point = strrchr(fichier, '.');
  if (!point || point == fichier) return "";
  return point + 1;
}

int main(int argc, char* argv[]) {
  /* Test temporaire liste chainee */

  /* Test temporaire trie */
  printf("#### Test temporaire trie\n");
  Noeud_Trie* mon_trie = creer_noeud();
  insertion_trie(mon_trie, "hello");
  printf("%s\n", recherche_trie(mon_trie, "hello") ? "true" : "false");
  printf("%s\n", recherche_trie(mon_trie, "world") ? "true" : "false");
  liberer_trie(mon_trie);

  /* Test temporaire hashmap */
  printf("#### Test temporaire hashmap\n");
  const unsigned taille_initiale = 2;
  struct hashmap_s hashmap;
  if (0 != hashmap_create(taille_initiale, &hashmap)) {
    fprintf(stderr, "Erreur create\n");
  }
  char* cle = "hello";
  int valeur = 23;
  if (0 != hashmap_put(&hashmap, cle, strlen(cle), &valeur)) {
    fprintf(stderr, "Erreur put\n");
  }
  void* const element = hashmap_get(&hashmap, cle, strlen(cle));
  if (NULL == element) {
    fprintf(stderr, "Erreur el1\n");
  }
  void* const element2 = hashmap_get(&hashmap, "world", strlen("world"));
  if (NULL == element2) {
    fprintf(stderr, "Erreur el2\n");
  }
  hashmap_destroy(&hashmap);

  /* Main */
  printf("#### Main\n");

  // Recuperer les arguments
  bool l_flag = false, t_flag = false, h_flag = false;
  bool options_existe = false;
  int index, c;
  char* structure_a_utiliser;
  opterr = 0;

  while ((c = getopt(argc, argv, "lth:")) != -1) switch (c) {
      case 'l':
        l_flag = true;
        break;
      case 't':
        t_flag = true;
        break;
      case 'h':
        h_flag = true;
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
  printf("l_flag = %d, t_flag = %d, h_flag = %d, argc = %d\n ", l_flag, t_flag,
         h_flag, argc);

  // Appeller la fonction decompresser ou compresser en fonction de l'extension
  // du fichier et avec les options s'il y en a
  if (options_existe && argc == 3) {
    structure_a_utiliser = l_flag   ? "liste_chainee"
                           : t_flag ? "trie"
                                    : "hashmap";
    if (strcmp(extension_fichier(argv[2]), "txt") == 0) {
      lzw_compresser(argv[2], structure_a_utiliser);
    };
    if (strcmp(extension_fichier(argv[2]), "lzw") == 0) {
      lzw_decompresser(argv[2], structure_a_utiliser);
    };
  } else if (argc = 2) {
    if (strcmp(extension_fichier(argv[1]), "txt") == 0) {
      lzw_compresser(argv[1], "optimale");
    };
    if (strcmp(extension_fichier(argv[1]), "lzw") == 0) {
      lzw_decompresser(argv[1], "optimale");
    };
  } else {
    fprintf(stderr,
            "Nombre d'arguments incorrects. Utilisation: /bin/lzw [options] "
            "<chemin_fichier>");
  }

  return 0;
}