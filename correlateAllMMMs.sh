#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "Usage: ./correlateAllMMMs.sh <runID>" >&2
    exit 2
fi

runID=$1
echo $runID
mkdir ./run$runID
for (( b = 0 ; b <= 3; b++ )) ### Inner for loop ###
do
     echo At board $b ...
    ./correlateOneMMM.sh $runID input$b.dat > outlist.$b
     mkdir ./run$1/det$b
     mv ./*.png ./run$1/det$b/
     mv ./outlist.$b ./run$1/det$b/
done
echo ""
