#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <semaphore.h>

using namespace std;

const int MAX_CUSTOMERS = 10;
const int NUM_CHAIRS = 5;

sem_t customers;
sem_t barber;
sem_t mut_lock;

int num_customers = 0;
int barber_active = true;

void get_haircut(int customer_id) {
    sem_wait(&mut_lock);

    if (num_customers < NUM_CHAIRS) {
        num_customers++;
        cout << "Customer " << customer_id << " takes a seat in the waiting room." << endl;

        sem_post(&customers);
        sem_post(&mut_lock);
        sem_wait(&barber);

        cout << "Customer " << customer_id << " gets a haircut." << endl;
    } else {
        cout << "Customer " << customer_id << " leaves because there are no free chairs." << endl;
        sem_post(&mut_lock);
    }
}

void cut_hair() {
    while (barber_active) {
        sem_wait(&customers);
        sem_wait(&mut_lock);

        num_customers--;

        sem_post(&barber);
        sem_post(&mut_lock);

        cout << "Barber cuts hair." << endl;

        sleep(1);
    }
}

int main() {
    sem_init(&customers, 0, 0);
    sem_init(&barber, 0, 0);
    sem_init(&mut_lock, 0, 1);

    thread barber_thread(cut_hair);
    queue<thread> customer_threads;

    for (int i = 0; i < MAX_CUSTOMERS; i++) {
        customer_threads.push(thread(get_haircut, i + 1));
        // sleep(1);
    }

    while (!customer_threads.empty()) {
        customer_threads.front().join();
        customer_threads.pop();
    }

    sem_post(&customers);
    barber_active = false;
    barber_thread.join();

    sem_destroy(&customers);
    sem_destroy(&barber);
    sem_destroy(&mut_lock);

    return 0;
}
