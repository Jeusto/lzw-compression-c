// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// typedef struct maillon {
//   char* cle;
//   int valeur;
//   struct maillon* s;
// } maillon, *liste;

// liste creer_liste() { return NULL; }

// int est_vide(liste l) { return NULL == l; }

// char* concatener(char* s, char c) {
//   s[strlen(s)] = c;
//   s[strlen(s) + 1] = 0;
//   return s;
// }

// liste recherche(char* string, liste l) {
//   liste p;
//   for (p = l; p != NULL; p = p->s)
//     if (strcmp(p->cle, string) == 0) return p;
//   return NULL;
// }

// liste adjonction_tete(char* string, int code, liste l) {
//   liste p = (liste)(malloc(sizeof(maillon)));
//   if (p == NULL) {
//     return NULL;
//   }
//   p->cle = malloc(strlen(string) + 1);
//   strcpy(p->cle, string);
//   p->valeur = code;
//   p->s = l;
//   return p;
// }

// liste initialisation(liste dictio) {
//   char tmp[2];
//   for (int code = 255; code >= 0; code--) {
//     tmp[0] = code;
//     tmp[1] = 0;
//     dictio = adjonction_tete(tmp, code, dictio);
//   }
//   return dictio;
// }

// liste suppr_tete(liste l) {
//   if (!est_vide(l)) {
//     liste tmp = l;
//     l = l->s;
//     free(tmp->cle);
//     free(tmp);
//   }
//   return l;
// }

// liste vider(liste l) {
//   liste p = creer_liste();
//   for (p = l; p != NULL;) p = suppr_tete(p);
//   return NULL;
// }

// void compression(const char* fichier) {
//   int c, b, code;
//   char tmp[512];
//   FILE* f1;
//   FILE* f2;

//   if ((f1 = fopen(fichier, "r")) == NULL) {
//     printf("problem sur fopen");
//     exit(0);
//   }

//   if ((f2 = fopen("file.lzw", "w+")) == NULL) {
//     printf("problem sur fopen");
//     exit(0);
//   }

//   liste dictio = creer_liste();
//   dictio = initialisation(dictio);
//   code = 256;
//   tmp[0] = '\0';
//   while ((c = fgetc(f1)) != EOF && b != EOF) {
//     concatener(tmp, c);
//     if (!recherche(tmp, dictio)) {
//       dictio = adjonction_tete(tmp, code++, dictio);
//       tmp[strlen(tmp) - 1] = 0;
//       int v = recherche(tmp, dictio)->valeur;
//       b = putc(v, f2);
//       tmp[0] = c;
//       tmp[1] = 0;
//     }
//   }
//   fclose(f1);  // A VERIF
//   fclose(f2);  // A VERIF
//   dictio = vider(dictio);
// }

// int main(int argc, char const* argv[]) {
//   if (argc != 2) {
//     fprintf(stderr, "%sArgument manquant\n", argv[0]);
//     exit(0);
//   }

//   compression(argv[1]);
//   return 0;
// }