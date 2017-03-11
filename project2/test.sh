#!/bin/bash

let count1=0;
let count2=0;

for f in $(ls ./test_cases/*.txt); do
	FNAME=$(basename $f .txt)
	{ ./a.out < $f &> ./test_cases/$FNAME.output; } 2> /dev/null;
	if [ $? -eq 139 ]; then
		touch ./test_cases/$FNAME.segfault
	fi
	diff -Bw  ./test_cases/$FNAME.output  ${f}.expected > ./test_cases/$FNAME.diff;
done

for f in $(ls test_cases/*.txt); do
	FNAME=$(basename $f .txt)
	echo "========================================================";
	echo "TEST CASE:" $FNAME;
	echo "========================================================";

	if [ -f ./test_cases/$FNAME.segfault ]; then
		echo "Segmentation fault (core dumped) when running your program on this test case"
		echo "Test: Failed"
	else
		d1=./test_cases/$FNAME.diff;
		if [ -s $d1 ]; then
			echo "There is an output missmatch:"
			cat $d1
		else
			count1=$((count1 + 1));
			echo "Test: Passed";
		fi
	fi
done

echo
echo "Tests passed:" $count1;

rm test_cases/*.output
rm test_cases/*.diff
rm -f test_cases/*.segfault
