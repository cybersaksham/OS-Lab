#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;
#define N 5

bool choosing[N];
int tickets[N];
int maxTicket = 0;

int done = false;

void *thread(void *arg) {
    int x = *(int *)arg;
    
    while(!done) {
        choosing[x] = true;
        tickets[x] = maxTicket + 1;
        maxTicket++;
        choosing[x] = false;

        for(int i=0; i<N; i++) {
            if(i == x) continue;
            while(choosing[i]);
            while(tickets[i] != 0 && tickets[i] < tickets[x]);
            if(tickets[i] == tickets[x] && i < x) {
                while(tickets[i] != 0);
            }
        }

        // Critical Section Code
        cout << "Process " << (x + 1) << " is in the critical section" << endl;

        tickets[x] = 0;
    }

    return NULL;
}

int main() {
    for(int i=0; i<N; i++) {
        choosing[i] = false;
        tickets[i] = 0;
    }

    pthread_t tids[N];

    for(int i=0; i<N; i++) {
        int *a = new int;
        *a = i;
        pthread_create(tids + i, NULL, thread, (void *)a);
    }

    for(int i=0; i<N; i++) {
        pthread_join(tids[i], NULL);
    }

    return 0;
}
