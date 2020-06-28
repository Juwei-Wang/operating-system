#include <iostream>
#include <thread>
#include <unistd.h>
#include <math.h>
//using namespace std;
long result[10];
void sum(long first, long end, int id)
{
// std::cout << "Thread #" << id << std::endl;
    long s=0;
    for (long i=first; i<end; i++) s+=i;
// Store result
    result[id] = s;
}
int main(int argc, char *argv[])
{
    std::cout << "Hello!" << std::endl;
    int nThreads = atoi(argv[1]);
    long number = 1000000000;
    long partitionSize = floor(number/nThreads);
    std::thread threadPool[nThreads];
    for (int i = 0; i < nThreads; i++)
    {
// in case partitioning is not perfectly even
        if (i == nThreads - 1)
            threadPool[i] = std::thread(sum, i*partitionSize, number, i);
        else
            threadPool[i] = std::thread(sum, i*partitionSize,
                                        (i+1)*partitionSize, i);
    }
// wait for every thread to finish
    for (int i = 0; i < nThreads; i++)
    {
        threadPool[i].join();
    }
// Add up result array
    long total = 0;
    for (int i = 0; i < nThreads; i++)
        total += result[i];
    std::cout<<"Sum = "<<total << std::endl;
    return 0;
}