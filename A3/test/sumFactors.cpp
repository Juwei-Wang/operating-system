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
// juwei wang 30053278

std::mutex mtx; // mutex for critical section
std::condition_variable cv; // condition variable for critical section
bool ready = false; // Tell threads to run
int current = 0; // current count
long result[10000];
// function handle large number
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
    // lock the critical section to avoid race condition
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
   // std::cout << "I am thread No." << threadNum << "\n";
   // std::cout << start2 << "\n";
   // std::cout << end2 << "\n";
   //unlock the mtx, give the lock to other thread
    mtx.unlock();

    //std::cout << start2 << "for threadNum" << threadNum << "\n";
    //std::cout << end2 << "for threadNum" << threadNum << "\n";
    //std::cout << ready;
    while( start2 <= end2 && ready ) {
        if (n % start2 == 0){
            ready = false;
            //std::cout << "I get into here 1" <<"\n";
            result[index] =  start2;
            return 0;
        }
        if (n % (start2+2) == 0){
            ready = false;
            //std::cout << "I get into here 2" << "\n";
            result[index] =  start2 + 2;
            return 0;
        }
        start2 += 6;
    }
    //std::cout << threadNum << "finish" << endl;
    return 0;
}
//handle small number
int64_t getSmallestDivisor0(int64_t n, int index)
{
    if( n <= 3) {
        result[index] = 0;
        return 0;
    }// 2 and 3 are primes
    if( n % 2 == 0) {
        result[index] = 2;
        return 2;
    }    // handle multiples of 2
    if( n % 3 == 0){
        result[index] = 3;
        return 3;
    }
    // handle multiples of 3
    int64_t i = 5;
    int64_t max = sqrt(n);
    //std::cout << i << "\n";
    //std::cout << max << "\n";
    while( i <= max) {
        if (n % i == 0) {
            result[index] = i;
            return i;
        }
        if (n % (i+2) == 0){
            result[index] = i + 2;
            return i + 2;
        }

        i += 6;
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
        exit(-1);
    }
   // if( nThreads != 1) {
   //     std::cout << "Error: I am not multithreaded yet :-(\n";
   //     exit(-1);
   // }

    std::cout << "Using " << nThreads
              << " thread" << (nThreads == 1 ? "" : "s")
              << ".\n";
    //上面的不用管

    int first = 0;   //check the index of number
    int index = 0;   //check the index of number
    int64_t div = 0; //claim the number to read in
    std::thread threadPool[nThreads];
    /// sum up the divisors
    int64_t sum = 0;
    while( 1) {
        int64_t num;
        if( ! (std::cin >> num) ) break;
        
        //int64_t div = getSmallestDivisor(num);
        long camPa = pow((6*nThreads + 5),2);
        ready = true;   // create the threads to do the job
        if(num >= camPa ) {
            for (int i = 0; i < nThreads; ++i) {
                threadPool[i] = thread(getSmallestDivisor, num, i, nThreads, index);
            }


            for (int j = 0; j < nThreads; ++j) {
                threadPool[j].join();
            }
            // the line below may help you with debugging

        } else{
            threadPool[0] = thread(getSmallestDivisor0, num, index);
            threadPool[0].join();
        }

        div = result[index];
        index++;
        //std::cout << "Debug: " << num << " --> " << div << "\n";
    }
    // add all the results
    for (int i = 0; i < 10000; ++i) {
        sum += result[i];
    }

    /// report results
    std::cout << "Sum of divisors = " << sum << "\n";

    return 0;
}
