# mkdir dump
# mkdir ./dump/aho_dump
# mkdir ./dump/ehog_dump
s=(
    # clementina 
    # sinensis 
    trifoliata 
    # elegans 
    # A_hydrophila_HiSeq
    # B_cereus_HiSeq
    # B_cereus_MiSeq
    # B_fragilis_HiSeq 
    # M_abscessus_HiSeq 
    # M_abscessus_MiSeq
    # R_sphaeroides_HiSeq 
    # R_sphaeroides_MiSeq 
    # S_aureus_HiSeq
    # V_cholerae_HiSeq 
    # V_cholerae_MiSeq
    # X_axonopodis_HiSeq
    ) 

algos=(
    SSP
    SK
    EC
    BCER
)

format=",%M,%e,%U,%S"
format2=",%M"
# ./bin/AHO trifoliata > output
# ./bin/EHOG trifoliata >> output
# ./bin/SSPAHO trifoliata >> output
# ./bin/SSPEHOG trifoliata >> output

rm output
rm outputmem 
for dataset in ${s[@]};do
    /usr/bin/time -f "%M" --output=outputmem -a ./bin/AHO trifoliata >> output
    /usr/bin/time -f "%M" --output=outputmem -a ./bin/EHOG trifoliata >> output
    for algo in ${algos[@]};do
        myvar=$algo
        myvar+="AHO"
        timeout 1h /usr/bin/time -f "aa,%M" --output=outputmem -a ./bin/$myvar $dataset >> output
        if [ $? -gt 0 ]; then
            echo -n ",timeout,timeout" >> output 
            echo "timeout" >> outputmem 
        fi
        myvar2=$algo
        myvar2+="EHOG"
        timeout 1h /usr/bin/time -f "ae,%M" --output=outputmem -a ./bin/$myvar2 $dataset >> output
        if [ $? -gt 0 ]; then
            echo -n ",timeout,timeout" >> output 
            echo "timeout" >> outputmem 
            break
        fi
    done
    echo "" >> output
done