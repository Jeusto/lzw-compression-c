#!/bin/sh

# Messages d'erreur avec la langue francaise
if [ -z `locale -a | grep fr_FR.utf8` ]; then
    echo "Erreur - installez la langue fr_FR.utf8 pour lancer les tests"
    exit 1
fi

PROG="./lzw"
TMP="./tmp"

# Teste vide
check_empty ()
{
    if [ -s $1 ]; then
        return 0;
    fi
    
    return 1
}

# Teste echec du programme
# - Code de retour du pg doit etre egal a 1
# - Stdout doit etre vide
# - Stderr doit contenir un message d'erreur
check_echec()
{
    if [ $1 -ne 1 ]; then
        echo "Echec => code de retour == 0 alors que 1 attendu"
        return 0
    fi
    
    if check_empty $TMP/stdout; then
        echo "Echec => sortie standard non vide"
        return 0
    fi
    
    if ! check_empty $TMP/stderr; then
        echo "Echec => sortie erreur vide"
        return 0
    fi
    
    return 1
}

# Teste si le pg a reussi
# - Code de retour du pg doit etre egal a 0
# - Stdout et stderr doivent etre vides
check_success()
{
    if [ $1 -ne 0 ]; then
        echo "Echec => code de retour == 1 alors que 0 attendu"
        return 0
    fi
    
    
    if check_empty $TMP/stderr; then
        echo "Echec => sortie erreur non vide"
        return 0
    fi
    
    return 1
}

# Compare deux fichiers avec la commande cmp
cmp_fichiers()
{
    cmp $1 $2 > $TMP/cmp 2>&1
    
    if ! cmp $TMP/cmp $TMP/stderr > /dev/null 2>&1; then
        echo "Sortie differente de la commande cmp"
        echo -n "Sortie du pg:\t"
        cat $TMP/stderr
        echo -n "Devrait etre :\t"
        cat $TMP/cmp
        return 0
    fi
    
    return 1
}

test_1()
{
    echo "Test 1 - tests sur les arguments du programme (mode $MODE)" 
    echo -n "Test 1.1 - sans argument.........................."
    $PROG                          > $TMP/stdout 2> $TMP/stderr
    if check_echec $?;                             then return 1; fi
    echo "OK"
    
    echo -n "Test 1.2 - 1 argument............................."
    $PROG argument              > $TMP/stdout 2> $TMP/stderr
    if check_echec $?;                             then return 1; fi
    echo "OK"
    
    echo -n "Test 1.3 - trop d'arguments......................."
    $PROG argument1 argument2 argument3  > $TMP/stdout 2> $TMP/stderr
    if check_echec $?;                             then return 1; fi
    echo "OK"
    
    echo -n "Test 1.4 - fichier inexistant....................."
    $PROG -s $MODE $TMP/inexistant.txt        > $TMP/stdout 2> $TMP/stderr
    if check_echec $?;                             then return 1; fi
    echo "OK"
    
    echo -n "Test 1.5 - mauvaise extension....................."
    touch $TMP/mauvais-extension.png;
    $PROG -s $MODE $TMP/mauvais-extension.png    > $TMP/stdout 2> $TMP/stderr
    if check_echec $?;                             then return 1; fi
    echo "OK"
}

test_2 ()
{

    echo "Test 2 - test sur les fichiers (mode $MODE)"

    echo -n "Test 2.1 - fichier vide..........................."
    touch $TMP/titi21.txt ;
    touch $TMP/toto21.txt ;
    $PROG -s $MODE $TMP/toto21.txt            > $TMP/stdout 2> $TMP/stderr
    if check_success $?;                                 then return 1; fi
    echo "OK"

    echo -n "Test 2.2 - fichier extremement court................"
    base64 /dev/urandom | head -c 10 > $TMP/titi22.txt ;
    cp $TMP/titi22.txt $TMP/toto22.txt ;
    $PROG -s $MODE $TMP/toto22.txt                  > $TMP/stdout 2> $TMP/stderr
    if check_success $?;                                 then return 1; fi
    $PROG -s $MODE $TMP/toto22.lzw              > $TMP/stdout 2> $TMP/stderr
    if check_success $?;                                 then return 1; fi
    if cmp_fichiers $TMP/toto22.txt $TMP/titi22.txt    ;  then return 1; fi 
    echo "OK"

    echo -n "Test 2.3 - fichier tres court....................."
    base64 /dev/urandom | head -c 100 > $TMP/titi23.txt ;
    cp $TMP/titi23.txt $TMP/toto23.txt ;
    $PROG -s $MODE $TMP/toto23.txt                  > $TMP/stdout 2> $TMP/stderr
    if check_success $?;                                 then return 1; fi
    $PROG -s $MODE $TMP/toto23.lzw              > $TMP/stdout 2> $TMP/stderr
    if check_success $?;                                 then return 1; fi
    if cmp_fichiers $TMP/toto23.txt $TMP/titi23.txt    ;  then return 1; fi 
    echo "OK"

    echo -n "Test 2.4 - fichier court.........................."
    base64 /dev/urandom | head -c 1000 > $TMP/titi24.txt ;
    cp $TMP/titi24.txt $TMP/toto24.txt ;
    $PROG -s $MODE $TMP/toto24.txt                  > $TMP/stdout 2> $TMP/stderr
    if check_success $?;                                 then return 1; fi
    $PROG -s $MODE $TMP/toto24.lzw              > $TMP/stdout 2> $TMP/stderr
    if check_success $?;                                 then return 1; fi
    if cmp_fichiers $TMP/toto24.txt $TMP/titi24.txt    ;  then return 1; fi 
    echo "OK"

    echo -n "Test 2.5 - fichier moyen.........................."
    base64 /dev/urandom | head -c 10000 > $TMP/titi25.txt ;
    cp $TMP/titi25.txt $TMP/toto25.txt ;
    $PROG -s $MODE $TMP/toto25.txt                  > $TMP/stdout 2> $TMP/stderr
    if check_success $?;                                 then return 1; fi
    $PROG -s $MODE $TMP/toto25.lzw              > $TMP/stdout 2> $TMP/stderr
    if check_success $?;                                 then return 1; fi
    if cmp_fichiers $TMP/toto25.txt $TMP/titi25.txt    ;  then return 1; fi 
    echo "OK"

    echo -n "Test 2.6 - fichier long..........................."
    base64 /dev/urandom | head -c 100000 > $TMP/titi26.txt ;
    cp $TMP/titi26.txt $TMP/toto26.txt ;
    $PROG -s $MODE $TMP/toto26.txt                  > $TMP/stdout 2> $TMP/stderr
    if check_success $?;                                 then return 1; fi
    $PROG -s $MODE $TMP/toto26.lzw              > $TMP/stdout 2> $TMP/stderr
    if check_success $?;                                 then return 1; fi
    if cmp_fichiers $TMP/toto26.txt $TMP/titi26.txt    ;  then return 1; fi 
    echo "OK"

    echo -n "Test 2.7 - fichier tres long......................"
    head -c 250000 ./tests/the-adventures-of-sherlock-holmes.txt > $TMP/titi27.txt ;    
    cp $TMP/titi27.txt $TMP/toto27.txt ;
    $PROG -s $MODE $TMP/toto27.txt                  > $TMP/stdout 2> $TMP/stderr
    if check_success $?;                                 then return 1; fi
    $PROG -s $MODE $TMP/toto27.lzw              > $TMP/stdout 2> $TMP/stderr
    if check_success $?;                                 then return 1; fi
    if cmp_fichiers $TMP/toto27.txt $TMP/titi27.txt    ;  then return 1; fi 
    echo "OK"

    echo -n "Test 2.8 - fichier extremement long..............."
    head -c 500000 ./tests/the-adventures-of-sherlock-holmes.txt > $TMP/titi28.txt ;
    cp $TMP/titi28.txt $TMP/toto28.txt ;
    $PROG -s $MODE $TMP/toto28.txt                  > $TMP/stdout 2> $TMP/stderr
    if check_success $?;                                 then return 1; fi
    $PROG -s $MODE $TMP/toto28.lzw              > $TMP/stdout 2> $TMP/stderr
    if check_success $?;                                 then return 1; fi
    if cmp_fichiers $TMP/toto28.txt $TMP/titi28.txt     ;  then return 1; fi 
    echo "OK"
}

# test_2 ()
# {

#     echo "Test 3 - divers tests"

#     echo -n "Test 2.1 - compression ..........................."
#     touch $TMP/titi21.txt ;

# } 

# Lance les series de tests de fonctionnement

rm -R $TMP
mkdir $TMP
MODE="hashmap"

for T in $(seq 1 2)
do
    if test_$T; then
        echo "== Test $T : OK ✅\n"
    else
        echo "== Test $T : ECHEC 🤬"
        return 1
    fi
done


rm -R $TMP
mkdir $TMP
MODE="trie"
for T in $(seq 1 2)
do
    if test_$T; then
        echo "== Test $T : OK ✅\n"
    else
        echo "== Test $T : ECHEC 🤬"
        return 1
    fi
done


rm -R $TMP
mkdir $TMP
MODE="liste-chainee"
for T in $(seq 1 2)
do
    if test_$T; then
        echo "== Test $T : OK ✅\n"
    else
        echo "== Test $T : ECHEC 🤬"
        return 1
    fi
done


rm -R $TMP