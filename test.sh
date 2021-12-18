#!/bin/bash

PROG=bin/lzw
TMP="/tmp/$$"

# Vérifie si les tests sont lancés dans le bon répertoire 
if ! test -x "./$PROG"; then
    echo "Exécutable $PROG introuvable dans ce répertoire"
    exit 1
fi


if [ -z `locale -a | grep fr_FR.utf8` ]; then
    echo "Erreur - installez la langue fr_FR.utf8 pour lancer les tests"
    exit 1
fi

check_empty ()
{
    if [ -s $1 ]; then
        return 0;
    fi

    return 1
}

check_echec()
{
    if [ $1 -ne 1 ]; then
        echo "échec => code de retour == 0 alors que 1 attendu"
        return 0
    fi

    if ! check_empty $TMP/stderr; then
        echo "échec => sortie erreur vide"
        return 0
    fi

    return 1
}


check_success()
{
    if [ $1 -ne 0 ]; then
        echo "échec => code de retour == 1 alors que 0 attendu"
        return 0
    fi

    if check_empty $TMP/stderr; then
        echo "échec => sortie erreur non vide"
        return 0
    fi

    return 1
}

check_output(){

    diff -q -s -b <(sort "$1") <(sort "$2") >/dev/null 2>&1

    if [ $? -ne 0 ]; then
        echo "Sorties Erronées :"
        echo "'$1'" et "'$2'" sont différents :
        diff -b -y <(sort "$1") <(sort "$2")
        return 1
    else
        echo " OK"
        return 0
    fi

}

tester_existance_dossier(){
    if test -d "$1"; then
        rm -r "$1"
    else 
        if test -e "$1"; then
            rm "$1"
        fi
    fi
    return 0

}

creation_dossier_test()
{
    mkdir -p "./tests/photos"
    mkdir -p "./tests/photos/vacances"
    mkdir -p "./tests/photos/vacances/2020"
    mkdir -p "./tests/photos/vacances/2021"
    mkdir -p "./tests/vide"
    mkdir -p "./tests/garbage/vide"
    mkdir -p "./tests/musiques/pop/2020"
    mkdir -p "./tests/musiques/pop/2021"
    mkdir -p "./tests/musiques/country/2015"
    mkdir -p "./tests/work/scripts"
    mkdir -p "./tests/documents"
    mkdir -p "./tests/documents/files"

    touch "./tests/photos/fichier1.tiff"
    touch "./tests/photos/vacances/2020/fichier2.jpeg"
    touch "./tests/photos/vacances/2021/fichier3.gif"


    touch "./tests/documents/fichier1.zip"
    touch "./tests/documents/fichier2.7z"
    touch "./tests/documents/fichier3.pdf"
    touch "./tests/documents/fichier4.pdf"
    touch "./tests/documents/fichier5.mp3"
    touch "./tests/documents/files/fichier6.toto"    
    touch "./tests/documents/files/fichier7.vide"
    touch "./tests/documents/files/fichier8.vide"
    touch "./tests/documents/files/fichierzip.pdf"
    touch "./tests/documents/files/fichiermp3.pdf"

    touch "./tests/work/scripts/fichier1.php"
    touch "./tests/work/scripts/fichier2.php"
    touch "./tests/work/scripts/fichier3.sh"
    touch "./tests/work/scripts/fichier4.sh"
    touch "./tests/musiques/country/2015/musique1.mp3"
    touch "./tests/musiques/country/2015/musique2.mp3"
    touch "./tests/musiques/pop/2021/musique1.mp3"

}


test_1()
{
    echo -e "\033[1mTest 1 - tests sur les arguments du programme\033[0m ....................."
    echo " - Test 1.1 - sans argument ......................................."
    "./$PROG"                          > $TMP/stdout 2> $TMP/stderr
    if check_echec $?;                             then return 1; fi
    echo " OK"

    echo " - Test 1.2 - trop d'arguments ...................................."
    "./$PROG" test.sh test.sh  > $TMP/stdout 2> $TMP/stderr
    if check_echec $?;                             then return 1; fi
    echo " OK"

    echo " - Test 1.3 - l'argument n'est pas un répertoire .................."
    "./$PROG" test.sh > $TMP/stdout 2> $TMP/stderr
    if check_echec $?;                             then return 1; fi
    echo " OK"

    echo " - Test 1.4 - l'argument n'existe pas ............................."
    "./$PROG" dossier > $TMP/stdout 2> $TMP/stderr
    if check_echec $?;                             then return 1; fi
    echo " OK"
}

test_2(){

    echo -e "\033[1mTest 2 - test de fontionnalité\033[0m"
    "./$PROG" "./tests" > "./etudiant.txt" 2> $TMP/stderr
    echo " - Test 2.1 - test code retour ...................................."
    if check_success $?;                           then return 1; fi
    echo " OK"
    echo " - Test 2.2 - test de la sortie du programme ......................"
    check_output "./correction.txt" "./etudiant.txt"
    echo " - Test 2.3 - test suppression tampon ............................."
    if ! test -e "tampon"; then
        echo " OK"
    else 
        echo " Le tampon n'est pas suprimé"
    fi

}


test_3(){
    echo -e "\033[1mTest 3 - test terminaison de proc_1 et proc_2\033[0m ....................."
    "./$PROG" "./test" > /dev/null 2>&1
    pgrep lister 
    if [[ $? -eq 0 ]]; then
        echo "Un des deux processus est encore en cours d'execution"
    else
        echo " OK"
    fi
}


test_4()
{
    echo -e "\033[1mTest 4 - test mémoire\033[0m ............................................."
    valgrind --leak-check=full --error-exitcode=100 "./$PROG" "./test" > /dev/null 2> $TMP/stderr
    test $? = 100 && echo "échec => log de valgrind dans $TMP/stderr" && return 1
    echo " OK"

    return 0
}

mkdir $TMP
# nettoyage avant de lancer les tests
tester_existance_dossier "./tests"
tester_existance_dossier "./etudiant.txt"
tester_existance_dossier "tampon"

creation_dossier_test
for i in `seq 1 4`; do test_$i; done

#suppression des fichiers et dossiers intermédiaires
rm -R $TMP
tester_existance_dossier "./tests"
tester_existance_dossier "./etudiant.txt"
