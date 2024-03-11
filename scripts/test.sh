format="k,n,ehog_size,ehog_time,hog_size,hog_time,tot_time,memory"

bash scripts/build.sh

mkdir -p plots/tmp
dump_path="plots/tmp/dump"

echo "fixed k"
tests=(5000000 $(seq 10000000 10000000 100000000))
for algo in "SSP" "SK" "EC"; do
    echo $algo
    output_path="plots/random_fixed_k_${algo}"
    echo $format > $output_path
    for n in ${tests[@]};do
        echo -n "" > $dump_path
        timed_out=false
        for t in {0..99}; do
            timeout 30 /usr/bin/time -f "%M" --output=$dump_path -a ./bin/$algo 100000 $n $t >> $dump_path
            if [ $? -gt 0 ]; then
                timed_out=true
                break
            fi
        done
        if [ "$timed_out" == true ]; then
            echo "timeout" >> $output_path
            break
        else 
            for i in {1..8}; do
                cat $dump_path | echo -n $(awk -v N=$i -v FS="," '{ sum += $N } END { if (NR > 0) printf("%.3f" ,sum / NR) }') >> $output_path
                if [ $i -lt 8 ]; then
                    echo -n ',' >> $output_path
                fi
            done
            echo "" >> $output_path
        fi
    done
done

echo "fixed n"
tests=(10000 15000 20000 25000 $(seq 30000 10000 90000) $(seq 100000 100000 1000000))
for algo in "SSP" "SK" "EC" "BCER"; do
    echo $algo
    output_path="plots/random_fixed_n_${algo}"
    echo $format > $output_path
    for k in ${tests[@]};do
        echo -n "" > $dump_path
        for t in {0..9}; do
            timeout 30 /usr/bin/time -f "%M" --output=$dump_path -a ./bin/$algo $k 10000000 $t >> $dump_path
            if [ $? -gt 0 ]; then
                break
            fi
        done
        if [ $? -gt 0 ]; then
            echo "timeout" >> $output_path
            break
        else 
            for i in {1..8}; do
                cat $dump_path | echo -n $(awk -v N=$i -v FS="," '{ sum += $N } END { if (NR > 0) printf("%.3f", sum / NR) }') >> $output_path
                if [ $i -lt 8 ]; then
                    echo -n ',' >> $output_path
                fi
            done
            echo "" >> $output_path
        fi
    done
done

