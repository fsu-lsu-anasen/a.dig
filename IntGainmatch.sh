#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "Usage: ./IntGainmatch.sh <runID>" >&2
	echo "Be sure to run ./correlateAllMMMs.sh <runID> successfully to generate the right input for this file!" >&2
    exit 2
fi

for (( i = 0; i < 5; i++ ))      ### Outer for loop ###
do
	./bin/findGainMatchFactors $1 $i >> outputFile
	echo "Done with SABRE detector no." $i
done
mv outputFile ./run$1/gainmatch.run$1
echo ""
