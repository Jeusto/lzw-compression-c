#!/bin/sh

# Messages d'erreur avec la langue francaise
if [ -z `locale -a | grep fr_FR.utf8` ]; then
    echo "Erreur - installez la langue fr_FR.utf8 pour lancer les tests"
    exit 1
fi

PROG="./lzw"
TMP="./temp"

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
    echo "Test 1 - test de performances"
    
    echo -n "Test 1.1 - fichier vide..........................."
    touch $TMP/titi.txt ;
    touch $TMP/toto.txt ;
    $PROG -s hashmap $TMP/toto.txt            > $TMP/stdout 2> $TMP/stderr
    if check_success $?;                                 then return 1; fi
    echo "OK"

    echo -n "Test 1.2 - fichier tres tres court......................"
    base64 /dev/urandom | head -c 10 > $TMP/titi.txt ;
    cp $TMP/titi.txt $TMP/toto.txt ;
    $PROG -s hashmap $TMP/toto.txt                  > $TMP/stdout 2> $TMP/stderr
    if check_success $?;                                 then return 1; fi
    $PROG -s hashmap $TMP/toto2.lzw              > $TMP/stdout 2> $TMP/stderr
    if check_success $?;                                 then return 1; fi
    cmp_fichiers $TMP/toto.txt $TMP/titi.txt 
    echo "OK"
    
    echo -n "Test 1.3 - fichier tres court.........................."
    base64 /dev/urandom | head -c 100 > $TMP/titi.txt ;
    cp $TMP/titi.txt $TMP/toto.txt ;
    $PROG -s hashmap $TMP/toto.txt                  > $TMP/stdout 2> $TMP/stderr
    if check_success $?;                                 then return 1; fi
    $PROG -s hashmap $TMP/toto2.lzw              > $TMP/stdout 2> $TMP/stderr
    if check_success $?;                                 then return 1; fi
    cmp_fichiers $TMP/toto.txt $TMP/titi.txt 
    echo "OK"

    echo -n "Test 1.4 - fichier court.........................."
    base64 /dev/urandom | head -c 1000 > $TMP/titi.txt ;
    cp $TMP/titi.txt $TMP/toto.txt ;
    $PROG -s hashmap $TMP/toto.txt                  > $TMP/stdout 2> $TMP/stderr
    if check_success $?;                                 then return 1; fi
    $PROG -s hashmap $TMP/toto2.lzw              > $TMP/stdout 2> $TMP/stderr
    if check_success $?;                                 then return 1; fi
    cmp_fichiers $TMP/toto.txt $TMP/titi.txt 
    echo "OK"


    echo -n "Test 1.4 - fichier moyen.........................."
    base64 /dev/urandom | head -c 10000 > $TMP/titi.txt ;
    cp $TMP/titi.txt $TMP/toto.txt ;
    $PROG -s hashmap $TMP/toto.txt                  > $TMP/stdout 2> $TMP/stderr
    if check_success $?;                                 then return 1; fi
    $PROG -s hashmap $TMP/toto2.lzw              > $TMP/stdout 2> $TMP/stderr
    if check_success $?;                                 then return 1; fi
    cmp_fichiers $TMP/toto.txt $TMP/titi.txt 
    echo "OK"

    # echo -n "Test 1.5 - fichier moyen avec un seul caractere..."
    # rm -rf $TMP/titi.txt ; rm -rf $TMP/toto.txt ;
    # touch $TMP/titi.txt ; touch $TMP/toto.txt ;
    # for i in `seq 1 10000`; do echo -n "A" >> $TMP/titi.txt ; done
    # cp $TMP/titi.txt $TMP/toto.txt ;
    # $PROG $TMP/toto.txt                  > $TMP/stdout 2> $TMP/stderr
    # if check_success $?;                                 then return 1; fi
    # $PROG $TMP/toto2.lzw              > $TMP/stdout 2> $TMP/stderr
    # if check_success $?;                                 then return 1; fi
    # cmp_fichiers $TMP/toto.txt $TMP/titi.txt 
    # echo "OK"

    echo -n "Test 1.6 - fichier long.........................."
    base64 /dev/urandom | head -c 100000 > $TMP/titi.txt ;
    cp $TMP/titi.txt $TMP/toto.txt ;
    $PROG -s hashmap $TMP/toto.txt                  > $TMP/stdout 2> $TMP/stderr
    if check_success $?;                                 then return 1; fi
    $PROG -s hashmap $TMP/toto2.lzw              > $TMP/stdout 2> $TMP/stderr
    if check_success $?;                                 then return 1; fi
    cmp_fichiers $TMP/toto.txt $TMP/titi.txt 
    echo "OK"

  echo -n "Test 1.6 - fichier tres long.........................."
      base64 /dev/urandom | head -c 500000 > $TMP/titi.txt ;
      cp $TMP/titi.txt $TMP/toto.txt ;
      $PROG -s hashmap $TMP/toto.txt                  > $TMP/stdout 2> $TMP/stderr
      if check_success $?;                                 then return 1; fi
      $PROG -s hashmap $TMP/toto2.lzw              > $TMP/stdout 2> $TMP/stderr
      if check_success $?;                                 then return 1; fi
      cmp_fichiers $TMP/toto.txt $TMP/titi.txt 
      echo "OK"
  }


# Repertoire temporaire pour stocker les fichiers et les sorties du programme
mkdir $TMP

# Liste chainee
for T in $(seq 1)
do
    if test_$T; then
        echo "== Test $T : ok $T/6\n"
    else
        echo "== Test $T : echec"
        return 1
    fi
done

# # Trie
# for T in $(seq 1)
# do
#     if test_$T; then
#         echo "== Test $T : ok $T/6\n"
#     else
#         echo "== Test $T : echec"
#         return 1
#     fi
# done
# rm -R $TMP


# # Hashmap
# for T in $(seq 1)
# do
#     if test_$T; then
#         echo "== Test $T : ok $T/6\n"
#     else
#         echo "== Test $T : echec"
#         return 1
#     fi
# done
# rm -R $TMP