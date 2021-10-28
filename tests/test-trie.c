#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../src/trie.h"

int main(int argc, char const* argv[]) {
  printf("Hello world");

  Noeud_Trie* mon_trie = creer_noeud();
  insertion_trie(mon_trie, "hello");
  printf("%s\n", recherche_trie(mon_trie, "hello") ? "true" : "false");
  printf("%s\n", recherche_trie(mon_trie, "yo") ? "true" : "false");
  liberer_trie(mon_trie);

  return 0;
}
