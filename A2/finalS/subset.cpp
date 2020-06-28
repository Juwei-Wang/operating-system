// single threaded program that reads in 32-bit integers from
// standard input, and then counds and prints out how many
// subsets of these integers sum up to 0 (zero).
//
// this program uses a trivial exponential time algorithm, by
// testing every possible subset (there are 2^N-1 subsets to
// test for N integers)
//
// the program uses global variables - to make it a bit easier
// to convert to a multi-threaded code for programmers
// that have not used pthreads before
// 30053278 Juwei wang

#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <thread>
#include <math.h>

// global variables are bad ...
// but they are acceptable for your first mulithreaded program
std::vector<long> a;
long count = 0;
long result[32]; // all the final result will be stored in the array for each thread

// for debugging purposes (if you want to print the combinations)
void print_sol(long comb)
{
    return; // comment this out if you want to see the subsets

   //for(size_t i = 0 ; i < a.size() ; i ++ ) {
   //    if( comb & 1)
   //         printf("%ld ", a[i]);
   //     comb /= 2;
   //}
   //printf("\n");
}

// test a combination 'comb'
// the binary representation of comb will be used to decide
// which numbers in a[] will be used to test the sum
void test(long comb, int k)
{
    long long sum = 0;
    long bits = comb;
    for(size_t i = 0 ; i < a.size() ; i ++ ) {
        if( bits & 1) // check lowest bit
            sum += a[i];
        bits >>= 1; // shift bits to the right
    }
    if( sum == 0) {
        print_sol(comb);
        long s = result[k];
        s ++;
        result[k] = s;
    }
}


// test all combinatsion in range [from, to)
void test_range( long from, long to, int k)
{
    for( long i = from ; i < to ; i ++)
        test(i,k);
}

int main( int argc, char ** argv)
{
    //
    // parse command line arguments
    //
    if( argc > 2) {
        printf("Error: expected at most 1 command line argument.\n");
        return -1;
    }
    long nThreads = 1;
    if( argc > 1) {
        // convert argv[1] to 'nThreads', report error if failed
        char * end = 0;
        errno = 0;
        nThreads = strtol( argv[1], & end, 10);
        if( * end != 0 || errno != 0) {
            printf("Expected integer, but you gave me '%s'\n", argv[1]);
            return -1;
        }
    }

    // to check if the commandline argument is not in proper range.
    if (nThreads > 32){
        printf("Expected integer should be LOWER than 32, but you gave me '%s'\n", argv[1]);
        printf("the number of threads will decrease to 32");
        nThreads = 32;
    }

    // to check if the commandline argument is not in proper range.
    if( nThreads < 1) {
        printf("Expected integer should be greater than 1, but you gave me '%s'\n", argv[1]);
        printf("the number of threads will increase to 1");
        nThreads = 1;
    }

    //
    // read all integers one by one and put them into the array
    //
    while(1) {
        long n;
        if( 1 != scanf("%ld", & n)) break;
        a.push_back(n);
    }
    //calculate the numbers of subsets and how many subsets will be computed for each thread
    long number = long(1) << a.size();
    long partitionSize = floor(number/nThreads);
    //check if the numbers of subsets is lower than nThreads
    if(number < nThreads){
        partitionSize = 1;
    }

    //create a threadPool
    std::thread threadPool[nThreads];

    // evenly partition
    for (int i = 0; i < nThreads; i++){// in case partitioning is not perfectly even
        if (i == nThreads - 1)
            threadPool[i] = std::thread(test_range, i*partitionSize, number, i);
        else
            threadPool[i] = std::thread(test_range, i*partitionSize,(i+1)*partitionSize, i);
    }


    // debug message
    printf( "Using %ld thread(s) on %lu numbers.\n",
            nThreads, a.size());

    //
    // to convert this into multithreaded code, you should
    // create threads here, and make each thread count different
    // subsets
    //
    // to make the code work without synchronization mechanisms,
    // you should make separate counters for each thread
    //

    //wait all threads stop their job
    count = 0;
    for (int i = 0; i < nThreads; i++)
    {
        threadPool[i].join();
    }

    // Add up result array
    for (int i = 0; i < nThreads; i++)
        count += result[i];
    //test_range(1, long(1) << a.size()); // range = 1 .. 2^N using bitshift

    //empty set should not be included
    count = count - 1;
    //
    // once you join the threads, you can simply sum up the counters
    // to get the total count
    //
    // since this version does not use multiple counters, report the only
    // one we have
    //
    printf("Subsets found: %ld\n", count);

    return 0;
}