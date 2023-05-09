#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Usage: ./correlateOneMMM.sh <runID> <inputfile>" >&2
    exit 2
fi


r=0
w=0
runID=$1
inputfile=$2

for (( r = 0; r <= 15; r++ ))      ### Outer for loop: rings ###
do
    for (( w = 0 ; w <= 15; w++ )) ### Inner for loop: wedges ###
    do
	  echo -ne "\rAt ring: $r and wedge $w ...            " >&2
	  ./bin/findCorrelations $runID $inputfile $r $w
    done
done
echo "" >&2
