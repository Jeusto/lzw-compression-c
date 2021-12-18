# Compression de fichiers avec l’algorithme LZW

## Introduction

Projet réalisé dans le cadre de la Licence d'informatique à l'Universite de Strasbourg.
Implémentation d'une méthode de compression et de décompressions de fichiers textes correspondant à l'Algorithme LZW (Lempel-Ziv-Welch).

## Compilation et éxecution

Pour compiler utilise la commande `make`

Pour lancer le programme, veuillez éxecuter la commande `./bin/lwz [options] <nom_fichier>`

La seule option disponible est `-s` qui permet de préciser la structure de données à utiliser pour le dictionnaire. Les valeurs possibles sont `liste-chainee`, `trie` et `hashmap`.

Si l'extension du fichier est `".txt"`, le fichier sera compréssé et produira un fichier avec le même nom mais avec l'extension `".lzw"` (inversement si l'extension du fichier est `".lzw"`.

## Notes

Toutes les explications et procédures d'exécutions sont disponibles dans le rapport.

## Crédits

Projet réalisé par Saday Arhun et ALCINDOR Jodel.
