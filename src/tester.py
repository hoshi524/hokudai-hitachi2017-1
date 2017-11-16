import subprocess

def exec(command,graph,seed):
    subprocess.call('./out/graph_generator.out ./testcase/testcase {} {}'.format(graph,seed),shell=True)
    subprocess.call('{} < ./testcase/testcase > ./testcase/result'.format(command),shell=True)
    score = int(subprocess.check_output('./out/score_evaluator.out ./testcase/testcase ./testcase/result',shell=True))
    print('command : {}, graph : {}, seed : {}, score : {}'.format(command,graph,seed,score))
    return score

subprocess.call('g++ --std=c++0x -W -Wall -Wno-sign-compare -O2 -s -pipe -mmmx -msse -msse2 -msse3 -o out/main.out src/main.cpp',shell=True)
subprocess.call('g++ --std=c++0x -W -Wall -Wno-sign-compare -O2 -s -pipe -mmmx -msse -msse2 -msse3 -o out/test.out src/test.cpp',shell=True)
main = './out/main.out'
test = './out/test.out'

main_sum = 0
test_sum = 0
for i in range(1000):
    main_sum += exec(main,0,i)
    test_sum += exec(test,0,i)
    main_sum += exec(main,1,i)
    test_sum += exec(test,1,i)
    print('main : {}'.format(main_sum))
    print('test : {}'.format(test_sum))