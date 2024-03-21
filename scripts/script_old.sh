mkdir ehog_dump
mkdir dump
rm ./dump/memprofbcer
rm ./dump/memprofec
rm ./dump/outputec
rm ./dump/outputbcer
rm ./dump/memprofehogx
rm ./dump/outputehogx
format="%M,%e,%U,%S"
s=(
    clementina 
    sinensis 
    trifoliata 
    elegans 
    # R_sphaeroides_MiSeq 
    # V_cholerae_MiSeq
    # M_abscessus_MiSeq
    # B_cereus_MiSeq
    # A_hydrophila_HiSeq
    # B_cereus_HiSeq
    # B_fragilis_HiSeq 
    # M_abscessus_HiSeq 
    # R_sphaeroides_HiSeq 
    # S_aureus_HiSeq
    # V_cholerae_HiSeq 
    # X_axonopodis_HiSeq
    ) 
cmake .
make BCER
make EC
make BCEREHOG

for dataset in ${s[@]};do
    /usr/bin/time -f "$format" --output=./dump/memprofehogx -a ./bin/BCEREHOG $dataset >> ./dump/outputehogx
    /usr/bin/time -f "$format" --output=./dump/memprofbcer -a ./bin/BCER $dataset >> ./dump/outputbcer
    /usr/bin/time -f "$format" --output=./dump/memprofec -a ./bin/EC $dataset >> ./dump/outputec
done