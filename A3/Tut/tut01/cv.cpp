#include <iostream> // std::cout
#include <thread> // std::thread
#include <mutex> // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
using namespace std;
mutex mtx;
condition_variable produce,consume;
int cargo = 0; // shared value by producers and consumers

void consumer () {
/*
unique_lock: an object that manages a mutex object
- unique_lock calls unlock on the mutex in its destructor.
Benefit: in the case you leave the scope that unique_lock is defined in
(maybe due to an exception being thrown), you can be sure that the mutex will
unlock.
*/
    unique_lock<mutex> lck(mtx); // create unique_lock with mt and lock mtx
     // if cargo is 0, then wait until there's cargo to consume
    while (cargo==0) consume.wait(lck); // cv consume calls wait on lck which has the mutex mtx.
    cout << cargo << '\n';
    cargo=0;
    produce.notify_one(); // signal (unblock) all threads waiting on the condition variable produce.
}


void producer (int id) {
    unique_lock<mutex> lck(mtx); // create unique_lock with mtx and lock mtx
// if cargo isn't empty, wait for it to be consumed
    while (cargo!=0) produce.wait(lck);
    cargo = id;
    consume.notify_one(); // signal (unblock) all threads waiting on the consume condition variable.
}

int main ()
{
// create 10 consumer threads, and 10 producer threads
    thread consumers[10],producers[10];
    for (int i=0; i<10; ++i) {
        consumers[i] = thread(consumer); // consumer thread calls consumer() function
        producers[i] = thread(producer,i+1); // producer thread calls producer() function
    }
// join them back:
    for (int i=0; i<10; ++i) {
        producers[i].join();
        consumers[i].join();
    }
    return 0;
}