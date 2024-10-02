#!/bin/bash

input_sizes=(5000 20000)

# num=(1 2 5 10 15 20 25 30)
num=(1 2 4)

merge=0
quick=1

for n in "${input_sizes[@]}"
do
    # echo "Size = $n":
    # echo "QuickSort:"
    for i in "${num[@]}"
    do
        # echo "np = $i"
        mpirun -np $i ./task.out $quick $n
    done
    # echo "MergeSort:"
    for i in "${num[@]}"
    do
        # echo "np = $i"
        mpirun -np $i ./task.out $merge $n
    done
    # echo ""
done