#include <fstream>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <thread>
using namespace std;

// File stream for the shared file
fstream sharedFile;

// Mutex and condition variable
mutex mtx;
condition_variable cv;

// Counters for readers and writers
int readers = 0;
int writers = 0;

// Reader thread function
void reader(int id) {
    unique_lock<mutex> lock(mtx);
    while (writers > 0) {
        cv.wait(lock);
    }
    readers++;
    lock.unlock();

    string line;
    sharedFile.seekg(0, ios::beg);
    while (getline(sharedFile, line)) {
        cout << "Reader " << id << " reads: " << line << endl;
    }

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

    sharedFile.clear();
    sharedFile.seekp(0, ios::beg);
    sharedFile << "Writer " << id << " writes" << endl;

    lock.lock();
    writers--;
    cv.notify_all();
    lock.unlock();
}

// Main function to create threads and run the program
int main() {
    const int numReaders = 50;
    const int numWriters = 20;
    thread readers[numReaders];
    thread writers[numWriters];

    sharedFile.open("sharedFile.txt", ios::in | ios::out | ios::app);

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

    sharedFile.close();

    return 0;
}
