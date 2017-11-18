import time
import subprocess

def exec(command,graph,seed):
    subprocess.call('./out/graph_generator.out ./testcase/testcase {} {}'.format(graph,seed),shell=True)
    start = time.time()
    subprocess.call('{} < ./testcase/testcase > ./testcase/result'.format(command),shell=True)
    end = time.time()
    score = int(subprocess.check_output('./out/score_evaluator.out ./testcase/testcase ./testcase/result',shell=True))
    print('command : {}, graph : {}, seed : {}, score : {}'.format(command,graph,seed,score))
    return [score, end - start]

subprocess.call('g++-5 -std=gnu++1y -O2 -o out/graph_generator.out toolkit/scripts/graph_generator.cpp',shell=True)
subprocess.call('g++-5 -std=gnu++1y -O2 -o out/main.out src/main.cpp',shell=True)
subprocess.call('g++-5 -std=gnu++1y -O2 -o out/test.out src/test.cpp',shell=True)
main = './out/main.out'
test = './out/test.out'

main_sum = 0
main_time = 0
test_sum = 0
test_time = 0
for i in range(1000):
    score, ela = exec(main,0,i)
    main_sum += score
    main_time += ela
    score, ela = exec(test,0,i)
    test_sum += score
    test_time += ela
    print('main : {}'.format(main_sum))
    print('test : {}'.format(test_sum))
    print('main : {}'.format(main_time))
    print('test : {}'.format(test_time))