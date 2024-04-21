bash scripts/build.sh

mkdir -p tmp
dump_path="tmp/dump"

format="k,n,aho_size,aho_time,aho_mem,ehog_size,ehog_time,ehog_mem,"
for algo in "SSP" "SK" "SP" "EC" "BCER"; do
    format="$format${algo}_time,${algo}_mem,"
done
echo "format is $format"

trials=100

echo "fixed k"
output_path="plots/random_fixed_k"
k=100000
tests=(5000000 $(seq 10000000 10000000 100000000))
echo $format > $output_path
for n in ${tests[@]};do
    echo "$k $n"

    echo "AHO"
    echo -n "" > $dump_path
    for ((t = 0; t < $trials; t++)); do
        /usr/bin/time -f "%M" --output=$dump_path -a ./bin/AHO $k $n $t >> $dump_path
    done
    for i in {1..5}; do
        cat $dump_path | echo -n $(awk -v N=$i -v FS="," '{ sum += $N } END { if (NR > 0) printf("%.3f" ,sum / NR) }') >> $output_path
        echo -n ',' >> $output_path
    done

    echo "EHOG"
    echo -n "" > $dump_path
    for ((t = 0; t < $trials; t++)); do
        /usr/bin/time -f "%M" --output=$dump_path -a ./bin/EHOG $k $n $t >> $dump_path
    done
    for i in {1..3}; do
        cat $dump_path | echo -n $(awk -v N=$i -v FS="," '{ sum += $N } END { if (NR > 0) printf("%.3f" ,sum / NR) }') >> $output_path
        echo -n ',' >> $output_path
    done

    for algo in "SSP" "SK" "SP" "EC" "BCER"; do
        echo $algo
        echo -n "" > $dump_path
        timed_out=false
        for ((t = 0; t < $trials; t++)); do
            timeout 30 /usr/bin/time -f "%M" --output=$dump_path -a ./bin/${algo}EHOG $k $n $t >> $dump_path
            if [ $? -gt 0 ]; then
                timed_out=true
                break
            fi
        done
        if [ "$timed_out" == true ]; then
            echo -n "timeout,0.000," >> $output_path
        else
            for i in {1..2}; do
                cat $dump_path | echo -n $(awk -v N=$i -v FS="," '{ sum += $N } END { if (NR > 0) printf("%.3f" ,sum / NR) }') >> $output_path
                echo -n ',' >> $output_path
            done
        fi
    done
    echo "" >> $output_path
done

echo "fixed n"
output_path="plots/random_fixed_n"
n=10000000
tests=(10000 15000 20000 25000 $(seq 30000 10000 90000) $(seq 100000 100000 1000000))
echo $format > $output_path
for k in ${tests[@]};do
    echo "$k $n"

    echo "AHO"
    echo -n "" > $dump_path
    for ((t = 0; t < $trials; t++)); do
        /usr/bin/time -f "%M" --output=$dump_path -a ./bin/AHO $k $n $t >> $dump_path
    done
    for i in {1..5}; do
        cat $dump_path | echo -n $(awk -v N=$i -v FS="," '{ sum += $N } END { if (NR > 0) printf("%.3f" ,sum / NR) }') >> $output_path
        echo -n ',' >> $output_path
    done

    echo "EHOG"
    echo -n "" > $dump_path
    for ((t = 0; t < $trials; t++)); do
        /usr/bin/time -f "%M" --output=$dump_path -a ./bin/EHOG $k $n $t >> $dump_path
    done
    for i in {1..3}; do
        cat $dump_path | echo -n $(awk -v N=$i -v FS="," '{ sum += $N } END { if (NR > 0) printf("%.3f" ,sum / NR) }') >> $output_path
        echo -n ',' >> $output_path
    done

    for algo in "SSP" "SK" "SP" "EC" "BCER"; do
        echo $algo
        echo -n "" > $dump_path
        timed_out=false
        for ((t = 0; t < $trials; t++)); do
            timeout 30 /usr/bin/time -f "%M" --output=$dump_path -a ./bin/${algo}EHOG $k $n $t >> $dump_path
            if [ $? -gt 0 ]; then
                timed_out=true
                break
            fi
        done
        if [ "$timed_out" == true ]; then
            echo -n "timeout,0.000," >> $output_path
        else
            for i in {1..2}; do
                cat $dump_path | echo -n $(awk -v N=$i -v FS="," '{ sum += $N } END { if (NR > 0) printf("%.3f" ,sum / NR) }') >> $output_path
                echo -n ',' >> $output_path
            done
        fi
    done
    echo "" >> $output_path
done
