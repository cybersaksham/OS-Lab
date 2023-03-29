#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;

bool p1_in = false;
bool p2_in = false;
int done1 = false;
int done2 = false;

int balance = 1000;
int w_amt = 200, d_amt = 300;

void *process1(void *) {
    while(!done1) {
        while(p2_in);
        sleep(1);
        p1_in = true;
        cout << "Process 1 in critical section" << endl;

        // Withdraw
        int bal = balance;
        sleep(1);
        int fin = bal - w_amt;
        balance = fin;

        done1 = true;
        p1_in = false;
    }
    return NULL;
}

void *process2(void *){
    while(!done2) {
        while(p1_in);
        p2_in = true;
        cout << "Process 2 in critical section" << endl;

        // Withdraw
        int bal = balance;
        sleep(1);
        int fin = bal + d_amt;
        balance = fin;

        done2 = true;
        p2_in = false;
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
