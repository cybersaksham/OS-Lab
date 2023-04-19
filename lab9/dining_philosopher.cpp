#include <iostream>
#include <thread>
#include <unistd.h>
#include <mutex>
#include <semaphore.h>

using namespace std;

const int NUM_PHILOSOPHERS = 5;
const int NUM_FORKS = 5;

mutex cout_mutex;
sem_t forks[NUM_FORKS];

void philosopher(int id) {
    while (true) {
        cout_mutex.lock();
        cout << "Philosopher " << id << " is thinking" << endl;
        cout_mutex.unlock();

        sleep(1);

        sem_wait(&forks[id]);
        sem_wait(&forks[(id + 1) % NUM_FORKS]);

        cout_mutex.lock();
        cout << "Philosopher " << id << " is eating" << endl;
        cout_mutex.unlock();

        sleep(1);

        sem_post(&forks[id]);
        sem_post(&forks[(id + 1) % NUM_FORKS]);
    }
}

int main() {
    for (int i = 0; i < NUM_FORKS; i++) {
        sem_init(&forks[i], 0, 1);
    }

    thread philosophers[NUM_PHILOSOPHERS];
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosophers[i] = thread(philosopher, i);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosophers[i].join();
    }

    for (int i = 0; i < NUM_FORKS; i++) {
        sem_destroy(&forks[i]);
    }

    return 0;
}
