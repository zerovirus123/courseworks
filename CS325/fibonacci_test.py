#test file for checking the runtime of the iterative/recursive version of the algorithm

import fibonacci_it
import fibonacci_re
import timeit

def main():

    input = [2,4,6,8,10,12,14,16,18,20,22,24,26,28,30]

    print "*****Fibonacci Iterative*****" + "\n"
    for item in input:
        print "Input #: " + str(item) + "\n"
        start = timeit.default_timer()
        str(fibonacci_it.fibonacci_it(item)) + "\n"
        stop = timeit.default_timer()
        print "Time taken: " + str(stop-start) + "\n"
        print "\n"

    print "*****Fibonacci Recursive*****" + "\n"
    for item in input:
        print "Input #: " + str(item) + "\n"
        start = timeit.default_timer()
        str(fibonacci_re.fibonacci_re(item)) + "\n"
        stop = timeit.default_timer()
        print "Time taken: " + str(stop-start) + "\n"
        print "\n"

main()