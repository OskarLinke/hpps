#!/bin/bash

WIDTH=1000
HEIGHT=1000
ROUNDS=1000

for n in {1..64}; do
sbatch --exclusive setup_environment.sh $PWD $n $WIDTH $HEIGHT $ROUNDS
done
