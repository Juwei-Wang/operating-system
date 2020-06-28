/// reads in a positive integers in range (2 .. 2^63-1) from standard input
/// for each number on input:
///   if the number is prime, it is ignored
///   determines the smallest factor of the composite number
/// prints out the sum of all smallest factors
///
/// if no composite numbers are found, prints out "0"
///
/// compile with:
///   $ g++ sumFactors.cpp -O3 -o gcd -lm

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cinttypes>
#include <cmath>
#include <iostream>
#include <thread>
#include <math.h>
#include <mutex>
#include <condition_variable>
using namespace std;

std::mutex mtx; // mutex for critical section
std::condition_variable cv; // condition variable for critical section
bool ready = false; // Tell threads to run
int current = 0; // current count

/// calculates smallest divisor of n that is less than n
///
/// returns 0 if n is prime or n < 2
///
/// examples:
///    n =  1         --> result = 0
///    n =  2         --> result = 0
///    n =  9         --> result = 3
///    n = 35         --> result = 5
///    n = 8589934591 --> result = 7
int64_t getSmallestDivisor(int64_t n, int threadNum, int nThreads)
{

    mtx.lock();
    if( n <= 3) {
        ready = false;
        return 0;
    }// 2 and 3 are primes
    if( n % 2 == 0) {
        ready = false;
        return 2;
    }// handle multiples of 2
    if( n % 3 == 0){
        ready = false;
        return 3;
    }

    // handle multiples of 3
    int64_t start1 = 5;
    //int64_t n = 110890;
    int64_t max = sqrt(n);
    int64_t diff = max - start1;
    int64_t num = diff/(6 * nThreads);
    // while(i <= max && ready != false)
    int64_t start2 = 5 + ((6 * num) * threadNum);
    int64_t end2 = 5 + ((6 * num) * (threadNum + 1));
    if(threadNum == (nThreads - 1)){
        end2 += (max - end2);
    }
    mtx.unlock();

    while( start2 <= end2 && ready ) {
        if (n % start2 == 0){
            ready = false;
            return start2;
        }
        if (n % start2 == 0){
            ready = false;
            return start2 + 2;
        }
        start2 += 6;
    }

    return 0;
}



int main( int argc, char ** argv)
{
    /// parse command line arguments
    int nThreads = 1;

    if( argc != 1 && argc != 2) {
        std::cout << "Usage: " << argv[0] << " [nThreads]\n";
        exit(-1);
    }

    if( argc == 2) nThreads = atoi( argv[1]);

    /// handle invalid arguments
    if( nThreads < 1 || nThreads > 256) {
        std::cout << "Warning: Bad arguments. 1 <= nThreads <= 256!\n";
    }
    if( nThreads != 1) {
        std::cout << "Error: I am not multithreaded yet :-(\n";
        exit(-1);
    }

    std::cout << "Using " << nThreads
              << " thread" << (nThreads == 1 ? "" : "s")
              << ".\n";

    //


    int first = 0;
    /// sum up the divisors
    int64_t sum = 0;
    while( 1) {
        int64_t num;
        if( ! (std::cin >> num) ) break;
        ready = true;
        if(first == 0 ){

        }else{

        }
        first = 1;

        int64_t div = getSmallestDivisor(num);
        // the line below may help you with debugging


        std::cout << "Debug: " << num << " --> " << div << "\n";
        sum += div;
    }

    /// report results
    std::cout << "Sum of divisors = " << sum << "\n";

    return 0;
}
