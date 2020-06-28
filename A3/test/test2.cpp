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
long result[1];

int64_t getSmallestDivisor(int64_t n, int threadNum, int nThreads, int index)
{

    
    if( n <= 3) {
        ready = false;
        result[index] =  0;
        return 0;
    }// 2 and 3 are primes
    if( n % 2 == 0) {
        ready = false;
        result[index] =  2;
        return 0;
    }// handle multiples of 2
    if( n % 3 == 0){
        ready = false;
        result[index] =  3;
        return 0;
    }
    mtx.lock();
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
            result[index] =  start2;
            return 0;
        }
        if (n % start2 == 0){
            ready = false;
            result[index] =  start2 + 2;
            return 0;
        }
        start2 += 6;
    }

    return 0;
}

int main()
{
    /// parse command line arguments
    int nThreads = 1;


    std::cout << "Using " << nThreads
              << " thread" << (nThreads == 1 ? "" : "s")
              << ".\n";

    //

    int first = 0;
    /// sum up the divisors
    int64_t sum = 0;
    int64_t num1 = 900720004735038979;
    int64_t num2 = 1;
    ready = true;

    thread threadPool[nThreads];
    thread t1 = thread(getSmallestDivisor, num1, 0, 1, 0);
    thread t2 = thread(getSmallestDivisor, num2, 0, 1, 1);

    t1.join();
    t2.join();

    sum = result[0] + result [1];
    /// report results
    std::cout << "Sum of divisors = " << sum << "\n";

    return 0;
}

