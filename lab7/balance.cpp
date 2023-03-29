#include <bits/stdc++.h>
using namespace std;

int balance = 1000;

int w1() {
    return balance;
}

int w2(int bal, int amt) {
    return bal - amt;
}

void w3(int fin) {
    balance = fin;
}

int d1() {
    return balance;
}

int d2(int bal, int amt) {
    return bal + amt;
}

void d3(int fin) {
    balance = fin;
}

int main() {
    cout << "Initial Balance: " << balance << endl;
    
    int w_amt = 200, d_amt = 300;

    // Sequence 1
    int bal1 = w1();
    int fin1 = w2(bal1, w_amt);
    int bal2 = d1();
    int fin2 = d2(bal2, d_amt);
    d3(fin2);
    w3(fin1);
    cout << "Final Balance 1: " << balance << endl;

    balance = 1000;
    // Sequence 2
    bal2 = d1();
    fin2 = d2(bal2, d_amt);
    bal1 = w1();
    fin1 = w2(bal1, w_amt);
    w3(fin1);
    d3(fin2);
    cout << "Final Balance 2: " << balance << endl;

    balance = 1000;
    // Sequence 3
    bal1 = w1();
    fin1 = w2(bal1, w_amt);
    w3(fin1);
    bal2 = d1();
    fin2 = d2(bal2, d_amt);
    d3(fin2);
    cout << "Final Balance 3: " << balance << endl;

    balance = 1000;
    // Sequence 4
    bal2 = d1();
    fin2 = d2(bal2, d_amt);
    d3(fin2);
    bal1 = w1();
    fin1 = w2(bal1, w_amt);
    w3(fin1);
    cout << "Final Balance 4: " << balance << endl;

    balance = 1000;
    // Sequence 5
    bal1 = w1();
    bal2 = d1();
    fin2 = d2(bal2, d_amt);
    fin1 = w2(bal1, w_amt);
    d3(fin2);
    w3(fin1);
    cout << "W1; D1; D2; W2; D3; W3: " << balance << endl;

    balance = 1000;
    // Sequence 6
    bal1 = w1();
    fin1 = w2(bal1, w_amt);
    bal2 = d1();
    w3(fin1);
    fin2 = d2(bal2, d_amt);
    d3(fin2);
    cout << "W1; W2; D1; W3; D2; D3: " << balance << endl;

    balance = 1000;
    // Sequence 7
    bal1 = w1();
    bal2 = d1();
    fin1 = w2(bal1, w_amt);
    fin2 = d2(bal2, d_amt);
    w3(fin1);
    d3(fin2);
    cout << "W1; D1; W2; D2; W3; D3: " << balance << endl;

    balance = 1000;
    // Sequence 9
    bal2 = d1();
    bal1 = w1();
    fin2 = d2(bal2, d_amt);
    fin1 = w2(bal1, w_amt);
    d3(fin2);
    w3(fin1);
    cout << "D1; W1; D2; W2; D3; W3: " << balance << endl;

    balance = 1000;
    // Sequence 9
    bal2 = d1();
    fin2 = d2(bal2, d_amt);
    bal1 = w1();
    fin1 = w2(bal1, w_amt);
    d3(fin2);
    w3(fin1);
    cout << "D1; D2; W1; W2; D3; W3: " << balance << endl;

    return 0;
}