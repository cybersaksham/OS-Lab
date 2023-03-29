#include <iostream>
#include <pthread.h>
using namespace std;

int processNumber = 1;
int done1 = false;
int done2 = false;

int balance = 1000;
int w_amt = 200, d_amt = 300;

void *process1(void *) {
    while(!done1) {
        while(processNumber != 1);
        cout << "Process 1 in critical section" << endl;

        // Withdraw
        int bal = balance;
        int fin = bal - w_amt;
        balance = fin;

        processNumber = 2;
        done1 = true;
    }
    return NULL;
}

void *process2(void *){
    while(!done2) {
        while(processNumber != 2);
        cout << "Process 2 in critical section" << endl;

        // Withdraw
        int bal = balance;
        int fin = bal + d_amt;
        balance = fin;

        processNumber = 1;
        done2 = true;
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

    cout << "Final Balance = " << balance << endl;

    return 0;
}
