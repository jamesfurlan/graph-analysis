make
for i in 1 2 3 4 5 6; do
    ../Ass2_Testing/testLanceWilliamsHAC ../Ass2_Testing/graphs/$i.in 1 |sort -n  > outFiles/SINGLE$i.out
    ./testLanceWilliamsHAC graphs/$i.in 2 |sort -n  > outFiles/COMPLETE$i.out
done
