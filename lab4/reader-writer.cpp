#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
using namespace std;

int sharedData = 0;
int readers = 0;
int writers = 0;
mutex mtx;
condition_variable cv;

// Reader thread function
void reader(int id) {
    unique_lock<mutex> lock(mtx);
    while (writers > 0) {
        cv.wait(lock);
    }
    readers++;
    lock.unlock();

    cout << "Reader " << id << " reads: " << sharedData << endl;

    lock.lock();
    readers--;
    if (readers == 0) {
        cv.notify_one();
    }
    lock.unlock();
}

// Writer thread function
void writer(int id) {
    unique_lock<mutex> lock(mtx);
    while (readers > 0 || writers > 0) {
        cv.wait(lock);
    }
    writers++;
    lock.unlock();

    sharedData = id;
    cout << "Writing... " << id << endl;

    lock.lock();
    writers--;
    cv.notify_all();
    lock.unlock();
}

int main() {
    const int numReaders = 50;
    const int numWriters = 50;
    thread readers[numReaders];
    thread writers[numWriters];

    for (int i = 0; i < max(numWriters, numReaders); i++) {
        if(i < numWriters) writers[i] = thread(writer, i);
        if(i < numReaders) readers[i] = thread(reader, i);
    }

    for (int i = 0; i < numReaders; i++) {
        readers[i].join();
    }
    for (int i = 0; i < numWriters; i++) {
        writers[i].join();
    }

    return 0;
}
