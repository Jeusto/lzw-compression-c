# Compression de fichiers avec l’algorithme LZW

## Introduction

Projet réalisé dans le cadre de la Licence d'informatique à l'Université de Strasbourg.
Implémentation d'une méthode de compression et de décompressions de fichiers textes correspondant à l'Algorithme LZW (Lempel-Ziv-Welch).

## Compilation et éxecution

Pour compiler, veuillez utiliser la commande `make`

Pour lancer le programme, veuillez éxecuter la commande `./lwz [options] <nom_fichier>`

Pour lancer les tests de fonctionnalité, veuillez exécuter la commande `make test-fonctionnalite`

Pour lancer les tests de de performances, veuillez exécuter la commande `make test-performance`

## Notes

La seule option disponible est `-s` qui permet de préciser la structure de données à utiliser pour le dictionnaire. Les valeurs possibles sont `liste-chainee`, `trie` et `hashmap`.
Par défaut, la compression et décompression se font en utilisant le `hashmap`.

Si l'extension du fichier est `".txt"`, le fichier sera compréssé et produira un fichier avec le même nom mais avec l'extension `".lzw"` (inversement si l'extension du fichier est `".lzw"`.

Toutes les explications et procédures d'exécutions sont disponibles dans le rapport.

## Crédits

Projet réalisé par SADAY Arhun et ALCINDOR Jodel.
