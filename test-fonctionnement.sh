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
    echo "Test 1 - tests sur les arguments du programme"
    
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
    $PROG -s hashmap $TMP/inexistant.txt        > $TMP/stdout 2> $TMP/stderr
    if check_echec $?;                             then return 1; fi
    echo "OK"
    
    echo -n "Test 1.5 - mauvaise extension....................."
    touch $TMP/mauvais-extension.png;
    $PROG -s hashmap $TMP/mauvais-extension.png    > $TMP/stdout 2> $TMP/stderr
    if check_echec $?;                             then return 1; fi
    echo "OK"
}

test_2 ()
{
    echo "Test 2 - tests sur des types de fichier"
    
    echo -n "Test 2.1 - fichier vide............."
    touch $TMP/titi.txt ;
    touch $TMP/toto.txt ;
    $PROG -s hashmap $TMP/toto.txt                  >   $TMP/stdout
    if check_success $?;                                    then return 0; fi
    echo "OK"
    
    echo -n "Test 2.2 - petit fichier aleatoire............."
    base64 /dev/urandom | head -c 100 > $TMP/titi.txt ;
    cpy $TMP/titi.txt $TMP/toto.txt ;
    # $PROG $TMP/titi $TMP/toto      > $TMP/stdout 2> $TMP/stderr
    # if check_echec $?;                            then return 1; fi
    # if cmp_sortie  $TMP/titi $TMP/toto;           then return 1; fi
    # echo "OK"
    
    # echo -n "Test 2.3 - petit fichier une seule caractere............."
    # $PROG /bin/ls /bin/ls          > $TMP/stdout 2> $TMP/stderr
    # if check_success $?;                          then return 1; fi
    # echo "OK"
    
    # echo -n "Test 2.4 - grand fichier aleatoire............."
    # cp /bin/ls $TMP/toto ; cp /bin/ls $TMP/titi
    # echo "a" >> $TMP/toto    ; echo "b" >> $TMP/titi
    # $PROG $TMP/titi $TMP/toto      > $TMP/stdout 2> $TMP/stderr
    # if check_echec $?;                            then return 1; fi
    # if cmp_sortie  $TMP/titi $TMP/toto;           then return 1; fi
    # echo "OK"
    
    # echo -n "Test 2.4 - grand fichier une seule caractere............."
    # cp /bin/ls $TMP/toto ; cp /bin/ls $TMP/titi
    # echo "a" >> $TMP/toto    ; echo "b" >> $TMP/titi
    # $PROG $TMP/titi $TMP/toto      > $TMP/stdout 2> $TMP/stderr
    # if check_echec $?;                            then return 1; fi
    # if cmp_sortie  $TMP/titi $TMP/toto;           then return 1; fi
    echo "OK"
}

test_3 ()
{
    # echo "Test 3 - tests sur des fichiers de taille diff."
    
    # echo -n "Test 3.1 - petits fichiers avec debut identique..."
    # echo -n "abc" > $TMP/toto ; echo -n "ab" > $TMP/titi
    # $PROG $TMP/toto $TMP/titi      > $TMP/stdout 2> $TMP/stderr
    # if check_echec $?;                            then return 1; fi
    # if cmp_sortie  $TMP/toto $TMP/titi;           then return 1; fi
    # echo "OK"
    
    # echo -n "Test 3.2 - grands fichiers avec debut identique..."
    # cp /bin/ls $TMP/toto ; echo -n "a" >> $TMP/toto
    # $PROG /bin/ls $TMP/toto        > $TMP/stdout 2> $TMP/stderr
    # if check_echec $?;                            then return 1; fi
    # if cmp_sortie /bin/ls $TMP/toto;              then return 1; fi
    # echo "OK"
    
    # echo -n "Test 3.3 - grands fichiers avec diff.............."
    # cp /bin/ls $TMP/toto ; sed "s/\(.\{4031\}\)./\1/" $TMP/toto > $TMP/titi
    # echo -n "a" >> $TMP/titi
    # $PROG $TMP/toto $TMP/titi      > $TMP/stdout 2> $TMP/stderr
    # if check_echec $?;                            then return 1; fi
    # if cmp_sortie  $TMP/toto $TMP/titi;           then return 1; fi
    # echo "OK"
    
    # echo -n "Test 3.4 - un fichier vide........................"
    # touch $TMP/tata
    # $PROG $TMP/toto $TMP/tata      > $TMP/stdout 2> $TMP/stderr
    # if check_echec $?;                            then return 1; fi
    # if cmp_sortie  $TMP/toto $TMP/tata;           then return 1; fi
    echo "OK"
}

test_4()
{
    #     echo -n "Test 4 - test memoire............................."
    #     valgrind --leak-check=full --error-exitcode=100 $PROG $TMP/titi $TMP/toto > /dev/null 2> $TMP/stderr
    #     test $? = 100 && echo "Echec => log de valgrind dans $TMP/stderr" && return 1
    #     echo "OK"
    
    return 0
}

# Repertoire temporaire pour stocker les fichiers et les sorties du programme
mkdir $TMP

# Lance les 4 series de tests
for T in $(seq 1 4)
do
    if test_$T; then
        echo "== Test $T : ok $T/4\n"
    else
        echo "== Test $T : echec"
        return 1
    fi
done

rm -R $TMP