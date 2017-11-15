set -e

cd `dirname $0`

g++ -o ./out/score_evaluator.out ./toolkit/scripts/score_evaluator.cpp
g++ --std=c++0x -W -Wall -Wno-sign-compare -O2 -s -pipe -mmmx -msse -msse2 -msse3 -o out/solve.out src/main.cpp

for i in `seq 1 100`
do
    ./out/solve.out < ./testcase/testcase-0-$i.in > ./testcase/result-0-$i.out
    ./out/score_evaluator.out ./testcase/testcase-0-$i.in ./testcase/result-0-$i.out
    ./out/solve.out < ./testcase/testcase-1-$i.in > ./testcase/result-1-$i.out
    ./out/score_evaluator.out ./testcase/testcase-1-$i.in ./testcase/result-1-$i.out
done
