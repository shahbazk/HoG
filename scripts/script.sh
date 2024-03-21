mkdir ehog_dump
mkdir dump
# rm ./dump/memprofssp
# rm ./dump/memprofsk
# rm ./dump/outputsk
# rm ./dump/outputssp
rm ./dump/memprofehog
rm ./dump/outputehog
format="%M,%e,%U,%S"
s=(
    clementina 
    sinensis 
    trifoliata 
    elegans 
    A_hydrophila_HiSeq
    B_cereus_HiSeq
    B_cereus_MiSeq
    B_fragilis_HiSeq 
    M_abscessus_HiSeq 
    M_abscessus_MiSeq
    R_sphaeroides_HiSeq 
    R_sphaeroides_MiSeq 
    S_aureus_HiSeq
    V_cholerae_HiSeq 
    V_cholerae_MiSeq
    X_axonopodis_HiSeq
    ) 
cmake .
make SSP
make SK
make SSPEHOG

for dataset in ${s[@]};do
    /usr/bin/time -f "$format" --output=./dump/memprofehog -a ./bin/SSPEHOG $dataset >> ./dump/outputehog
    /usr/bin/time -f "$format" --output=./dump/memprofssp -a ./bin/SSP $dataset >> ./dump/outputssp
    /usr/bin/time -f "$format" --output=./dump/memprofsk -a ./bin/SK $dataset >> ./dump/outputsk
done