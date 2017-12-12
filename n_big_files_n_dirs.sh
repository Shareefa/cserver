#!/bin/bash
basedir=$(pwd)

for outer in {0..50..5}
do
echo $outer
rm -rf dir1
rm -rf output
for((i=1; i<=$outer; i++))
do
mkdir "dir$i"
cd "dir$i"
cp "../movie.csv" movie.csv
done
cd $basedir
mkdir output
{ time ./threadsort -c director_name -o output -d dir1; } 2>> analysis.txt
done
exit 0
