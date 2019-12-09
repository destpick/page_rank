#!/bin/sh

for threads in 1 2 4 8
do
	echo "running"
	./run.sh NotreDame.txt 10 0.2 ${threads}

done
