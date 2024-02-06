

echo "fixed n"

for algo in "ssp" "sk"; do

    echo $algo

    make $algo

    for o in 0.1 0.5 0.9;do

        for i in {60..70};do

            /usr/bin/time -f "%M" ./bin/testHOG 50 $i $o

        done

    done

done

echo "fixed p"

for algo in "ssp" "sk"; do

    echo $algo

    make $algo

    for o in 0.1 0.5 0.9;do

        for i in {40..50};do

            /usr/bin/time -f "%M" ./bin/testHOG $i 70 $o

        done

    done

done
