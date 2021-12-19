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

test_1 ()
{    
    echo "\nTest de performance sur les fichiers (mode $MODE)\n"
    tempsPris=0
    moyenne=0




    for I in $(seq 1 5)
      do
        touch $TMP/titi21.txt ;
        touch $TMP/toto21.txt ;
        start_time=$(date +%s.%6N)
        $PROG -s $MODE $TMP/toto21.txt       > $TMP/stdout 2> $TMP/stderr
        $PROG -s $MODE $TMP/toto21.lzw         > $TMP/stdout 2> $TMP/stderr
        end_time=$(date +%s.%6N)
        tempsPris=$(echo "scale=6; $end_time - $start_time" | bc -l)
        echo -n "$tempsPris\t"
        echo "$tempsPris"  > $TMP/temps.time
      done

    moyenne=0
    while read value
    do
      moyenne=`echo "scale=3;${value} + ${moyenne}" | bc`         
    done < tmp/temps.time

    echo "\nTemps moyenne pour fichier vide (mode $MODE): $moyenne\n"
    echo "0 $moyenne" > "./resultats-tests/$MODE.time"




    for I in $(seq 1 5)
      do
        base64 /dev/urandom | head -c 10 > $TMP/titi22.txt ;
        cp $TMP/titi22.txt $TMP/toto22.txt ;
        start_time=$(date +%s.%6N)
        $PROG -s $MODE $TMP/toto22.txt                  > $TMP/stdout 2> $TMP/stderr
        $PROG -s $MODE $TMP/toto22.lzw              > $TMP/stdout 2> $TMP/stderr
        end_time=$(date +%s.%6N)
        tempsPris=$(echo "scale=6; $end_time - $start_time" | bc -l)
        echo -n "$tempsPris\t"
        echo "$tempsPris"  >> $TMP/temps.time
      done
    
    moyenne=0
    while read value
    do
      moyenne=`echo "scale=3;${value} + ${moyenne}" | bc`         
    done < tmp/temps.time

    echo "\nTemps moyenne pour fichier extremement court (mode $MODE): $moyenne\n"
    echo "10 $moyenne" >> "./resultats-tests/$MODE.time"




    for I in $(seq 1 5)
      do
        base64 /dev/urandom | head -c 100 > $TMP/titi23.txt ;
        cp $TMP/titi23.txt $TMP/toto23.txt ;
        start_time=$(date +%s.%6N)
        $PROG -s $MODE $TMP/toto23.txt                  > $TMP/stdout 2> $TMP/stderr
        $PROG -s $MODE $TMP/toto23.lzw              > $TMP/stdout 2> $TMP/stderr
        end_time=$(date +%s.%6N)
        tempsPris=$(echo "scale=6; $end_time - $start_time" | bc -l)
        echo -n "$tempsPris\t"
        echo "$tempsPris"  >> $TMP/temps.time
      done

    moyenne=0
    while read value
    do
      moyenne=`echo "scale=3;${value} + ${moyenne}" | bc`         
    done < tmp/temps.time

    echo "\nTemps moyenne pour fichier tres court(mode $MODE): $moyenne\n"
    echo "100 $moyenne" >> "./resultats-tests/$MODE.time"




    for I in $(seq 1 5)
      do
        base64 /dev/urandom | head -c 1000 > $TMP/titi24.txt ;
        cp $TMP/titi24.txt $TMP/toto24.txt ;
        start_time=$(date +%s.%6N)
        $PROG -s $MODE $TMP/toto24.txt                  > $TMP/stdout 2> $TMP/stderr
        $PROG -s $MODE $TMP/toto24.lzw              > $TMP/stdout 2> $TMP/stderr
        end_time=$(date +%s.%6N)
        tempsPris=$(echo "scale=6; $end_time - $start_time" | bc -l)
        echo -n "$tempsPris\t"
        echo "$tempsPris"  >> $TMP/temps.time
      done
    
    moyenne=0
    while read value
    do
      moyenne=`echo "scale=3;${value} + ${moyenne}" | bc`         
    done < tmp/temps.time

    echo "\nTemps moyenne pour fichier court (mode $MODE): $moyenne\n"
    echo "1000 $moyenne" >> "./resultats-tests/$MODE.time"




    for I in $(seq 1 5)
      do
        base64 /dev/urandom | head -c 10000 > $TMP/titi25.txt ;
        cp $TMP/titi25.txt $TMP/toto25.txt ;
        start_time=$(date +%s.%6N)
        $PROG -s $MODE $TMP/toto25.txt                  > $TMP/stdout 2> $TMP/stderr
        $PROG -s $MODE $TMP/toto25.lzw              > $TMP/stdout 2> $TMP/stderr
        end_time=$(date +%s.%6N)
        tempsPris=$(echo "scale=6; $end_time - $start_time" | bc -l)
        echo -n "$tempsPris\t"
        echo "$tempsPris"  >> $TMP/temps.time
      done

    moyenne=0
    while read value
    do
      moyenne=`echo "scale=3;${value} + ${moyenne}" | bc`         
    done < tmp/temps.time

    echo "\nTemps moyenne pour fichier moyen (mode $MODE): $moyenne\n"
    echo "10000 $moyenne" >> "./resultats-tests/$MODE.time"




    for I in $(seq 1 5)
      do
        base64 /dev/urandom | head -c 100000 > $TMP/titi26.txt ;
        cp $TMP/titi26.txt $TMP/toto26.txt ;
        start_time=$(date +%s.%6N)
        $PROG -s $MODE $TMP/toto26.txt                  > $TMP/stdout 2> $TMP/stderr
        $PROG -s $MODE $TMP/toto26.lzw              > $TMP/stdout 2> $TMP/stderr
        end_time=$(date +%s.%6N)
        tempsPris=$(echo "scale=6; $end_time - $start_time" | bc -l)
        echo -n "$tempsPris\t"
        echo "$tempsPris"  >> $TMP/temps.time
      done 
    
    moyenne=0
    while read value
    do
      moyenne=`echo "scale=3;${value} + ${moyenne}" | bc`         
    done < tmp/temps.time

    echo "\nTemps moyenne pour fichier long (mode $MODE): $moyenne\n"
    echo "100000 $moyenne" >> "./resultats-tests/$MODE.time"



    for I in $(seq 1 5)
      do
        head -c 250000 ./assets/the-adventures-of-sherlock-holmes.txt > $TMP/titi27.txt ;    
        cp $TMP/titi27.txt $TMP/toto27.txt ;
        start_time=$(date +%s.%6N)
        $PROG -s $MODE $TMP/toto27.txt                  > $TMP/stdout 2> $TMP/stderr
        $PROG -s $MODE $TMP/toto27.lzw              > $TMP/stdout 2> $TMP/stderr
        end_time=$(date +%s.%6N)
        tempsPris=$(echo "scale=6; $end_time - $start_time" | bc -l)
        echo -n "$tempsPris\t"
        echo "$tempsPris"  >> $TMP/temps.time
     done

    moyenne=0
    while read value
    do
      moyenne=`echo "scale=3;${value} + ${moyenne}" | bc`         
    done < tmp/temps.time

    echo "\nTemps moyenne pour fichier tres long (mode $MODE): $moyenne\n"
    echo "250000 $moyenne" >> "./resultats-tests/$MODE.time"




    for I in $(seq 1 5)
      do
        head -c 500000 ./assets/the-adventures-of-sherlock-holmes.txt > $TMP/titi28.txt ;
        cp $TMP/titi28.txt $TMP/toto28.txt ;
        start_time=$(date +%s.%6N)
        $PROG -s $MODE $TMP/toto28.txt                  > $TMP/stdout 2> $TMP/stderr
        $PROG -s $MODE $TMP/toto28.lzw              > $TMP/stdout 2> $TMP/stderr
        end_time=$(date +%s.%6N)
        tempsPris=$(echo "scale=6; $end_time - $start_time" | bc -l)
        echo -n "$tempsPris\t"
        echo "$tempsPris"  >> $TMP/temps.time
      done
      
    moyenne=0
    while read value
    do
      moyenne=`echo "scale=3;${value} + ${moyenne}" | bc`         
    done < tmp/temps.time

    echo "\nTemps moyenne pour fichier extrêmement (mode $MODE): $moyenne"
    echo "500000 $moyenne" >> "./resultats-tests/$MODE.time"
}

rm -R $TMP
mkdir $TMP
MODE="hashmap"
for T in $(seq 1)
do
    if test_$T; then
        echo "== Test performances avec $MODE : OK ✅\n"
    else
        echo "== Test performances avec $MODE : ECHEC 🤬"
        return 1
    fi
done

rm -R $TMP
mkdir $TMP
MODE="trie"
for T in $(seq 1)
do
    if test_$T; then
        echo "== Test performances avec $MODE : OK ✅\n"
    else
        echo "== Test performances avec $MODE : ECHEC 🤬"
        return 1
    fi
done

rm -R $TMP
mkdir $TMP
MODE="liste-chainee"
for T in $(seq 1)
do
    if test_$T; then
        echo "== Test performances avec $MODE : OK ✅\n"
    else
        echo "== Test  performances avec $MODE : ECHEC 🤬"
        return 1
    fi
done


rm -R $TMP