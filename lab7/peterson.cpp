#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;

int favoured = 1;
bool p1_wants = false;
bool p2_wants = false;
int done = false;

void *process1(void *) {
    while(!done) {
        p1_wants = true;
        favoured = 2;
        sleep(1);
        cout << "Process 1 waiting..." << endl;
        while(p2_wants && favoured == 2);
        cout << "Process 1 in critical section" << endl;

        // Critical Section

        favoured = 2;
        p1_wants = false;
    }
    return NULL;
}

void *process2(void *){
    while(!done) {
        p2_wants = true;
        favoured = 1;
        cout << "Process 2 waiting..." << endl;
        while(p1_wants && favoured == 1);
        cout << "Process 2 in critical section" << endl;

        // Critical Section

        favoured = 1;
        p2_wants = false;
    }
    return NULL;
}

int main() {
    pthread_t tid1, tid2;

    if (pthread_create(&tid1, NULL, process1, NULL)) {
        cout << "Error creating thread 1." << endl;
        return 1;
    }
    if (pthread_create(&tid2, NULL, process2, NULL)) {
        cout << "Error creating thread 2." << endl;
        return 1;
    }

    if (pthread_join(tid1, NULL)) {
        cout << "Error joining thread 1." << endl;
        return 1;
    }
    if (pthread_join(tid2, NULL)) {
        cout << "Error joining thread 2." << endl;
        return 1;
    }

    return 0;
}
