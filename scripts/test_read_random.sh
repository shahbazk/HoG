bash scripts/build.sh

mkdir -p tmp
dump_path="tmp/dump"

format="complete_len,snapshot_len,coverage,k,n,aho_size,aho_time,aho_mem,ehog_size,ehog_time,ehog_mem,"
for algo in "SSP" "SK" "SP" "EC" "BCER"; do
	format="$format${algo}_time,${algo}_mem,"
done
echo "format is $format"

trials=20

echo "fixed k"
output_path="plots/read_random"
complete_len_tests=(100000 1000000 10000000)
snapshot_len_tests=(500 1000 5000 10000)
coverage_tests=(30 50 100)
echo $format >$output_path
for complete_len in ${complete_len_tests[@]}; do
	for snapshot_len in ${snapshot_len_tests[@]}; do
		for coverage in ${coverage_tests[@]}; do
			echo "$complete_len $snapshot_len $coverage"

			echo "AHO"
			echo -n "" >$dump_path
			for ((t = 0; t < $trials; t++)); do
				/usr/bin/time -f "%M" --output=$dump_path -a ./bin/AHO $complete_len $snapshot_len $coverage $t >>$dump_path
			done
			for i in {1..8}; do
				cat $dump_path | echo -n $(awk -v N=$i -v FS="," '{ sum += $N } END { if (NR > 0) printf("%.3f" ,sum / NR) }') >>$output_path
				echo -n ',' >>$output_path
			done

			echo "EHOG"
			echo -n "" >$dump_path
			for ((t = 0; t < $trials; t++)); do
				/usr/bin/time -f "%M" --output=$dump_path -a ./bin/EHOG $complete_len $snapshot_len $coverage $t >>$dump_path
			done
			for i in {1..3}; do
				cat $dump_path | echo -n $(awk -v N=$i -v FS="," '{ sum += $N } END { if (NR > 0) printf("%.3f" ,sum / NR) }') >>$output_path
				echo -n ',' >>$output_path
			done

			for algo in "SSP" "SK" "SP" "EC" "BCER"; do
				echo $algo
				echo -n "" >$dump_path
				timed_out=false
				for ((t = 0; t < $trials; t++)); do
					timeout 30 /usr/bin/time -f "%M" --output=$dump_path -a ./bin/${algo}EHOG $complete_len $snapshot_len $coverage $t >>$dump_path
					if [ $? -gt 0 ]; then
						timed_out=true
						break
					fi
				done
				if [ "$timed_out" == true ]; then
					echo -n "timeout,0.000," >>$output_path
				else
					for i in {1..2}; do
						cat $dump_path | echo -n $(awk -v N=$i -v FS="," '{ sum += $N } END { if (NR > 0) printf("%.3f" ,sum / NR) }') >>$output_path
						echo -n ',' >>$output_path
					done
				fi
			done
			echo "" >>$output_path
		done
	done
done
