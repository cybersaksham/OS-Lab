#include<bits/stdc++.h>
using namespace std;

class Process{
public:
    int arrival = 0;
    int priority = 0;
    int startShare = 0;
    int endShare = 0;
    vector<pair<string, int>> burst;
};

int num_processes;
int quantum;
int lotteryTickets;
unordered_map<int, Process> processes;
queue<pair<int, Process>> process_queue;

void print(unordered_map<int, vector<int>> &time) {
    int total_turnaround = 0, total_response = 0, total_waiting = 0;
    for(auto it : time){
        int pid = it.first;
        cout << "\tPid : " << pid << ", ";
        cout << "Priority: " << processes[pid].priority << endl;
        int turnaround = 0, response = 0, waiting = 0;

        vector<int> bursts = it.second;
        cout << "\t\tBursts: ";
        for(auto burst : bursts) cout << burst << " ";
        cout << endl;

        turnaround = bursts[bursts.size() - 1] - processes[pid].arrival;
        response = bursts[0] - processes[pid].arrival;
        int burstTime = 0;
        for(int i=1; i<bursts.size(); i+=2){
            burstTime += bursts[i] - bursts[i - 1];
        }
        waiting = turnaround - burstTime;
        cout<<"\t\tArrival Time : " << processes[pid].arrival << endl;
        cout<<"\t\tTurnaround Time : " << turnaround << endl;
        cout<<"\t\tResponse Time : " << response << endl;
        cout<<"\t\tWaiting Time : "<< waiting << endl;
        total_turnaround += turnaround;
        total_response += response;
        total_waiting += waiting;
    }
    cout<<"\tAverage Turnaround Time : " << total_turnaround / (float)num_processes << endl;
    cout<<"\tAverage Response Time : " << total_response / (float)num_processes << endl;
    cout<<"\tAverage Waiting Time : " << total_waiting / (float)num_processes << endl;
}

void roundRobin(){
    unordered_map<int, Process> prs = processes;

    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<pair<int, pair<int, int>>>> priority;
    while(!process_queue.empty()) {
        int pid = process_queue.front().first;
        Process p = process_queue.front().second;
        process_queue.pop();
        priority.push({p.arrival, {pid, 0}});
    }

    queue<pair<int, pair<int, int>>> final_queue;
    while(!priority.empty()) {
        final_queue.push(priority.top());
        priority.pop();
    }

    unordered_map<int, vector<int>> time;
    int lastTime = -1;
    while(!final_queue.empty()) {
       int arrival = final_queue.front().first;
       int pid = final_queue.front().second.first;
       int i = final_queue.front().second.second;
       final_queue.pop();

       if(lastTime == -1) lastTime = arrival;

       if(i >= prs[pid].burst.size()) time[pid].push_back(lastTime);
       else if(prs[pid].burst[i].first == "P") {
            if(prs[pid].burst[i].second > quantum) {
                time[pid].push_back(lastTime);
                lastTime += quantum;
                time[pid].push_back(lastTime);
                arrival += quantum;
                prs[pid].burst[i].second -= quantum;
                final_queue.push({arrival, {pid, i}});
            }
            else {
                time[pid].push_back(lastTime);
                lastTime += prs[pid].burst[i].second;
                time[pid].push_back(lastTime);
                arrival = lastTime;
                final_queue.push({arrival, {pid, i + 1}});
            }
       } else {
            arrival += prs[pid].burst[i].second;
            final_queue.push({arrival, {pid, i + 1}});
       }
    }
    cout << endl << endl;
    cout <<"Round Robin :" << endl;
    print(time);
    cout << endl << endl;
}

void compSort(queue<pair<pair<int, int>, pair<int, int>>> &pq, int t){
    vector<pair<pair<int, int>, pair<int, int>>> v1, v2;
    while(!pq.empty()) {
        if(pq.front().first.first <= t){
            v1.push_back(pq.front());
        }
        else {
            v2.push_back(pq.front());
        }
        pq.pop();
    }

    sort(v1.begin(),v1.end(),[](pair<pair<int,int>,pair<int,int>>a,pair<pair<int,int>,pair<int,int>>b){
        return a.first.second<b.first.second;
    });
    sort(v2.begin(),v2.end(),[](pair<pair<int,int>,pair<int,int>>a,pair<pair<int,int>,pair<int,int>>b){
        if(a.first.first != b.first.first) return a.first.first < b.first.first;
        return a.first.second < b.first.second;
    });

    for(auto it:v1) pq.push(it);
    for(auto it:v2) pq.push(it);
}

int generateTicket() {
    return rand() % lotteryTickets;
}

void priorityNonPreEmptive(){
    unordered_map<int, Process> mp = processes;
    unordered_map<int, int> index;
    unordered_map<int, vector<int>> time;
    int lastTime = -1;

    vector<int> pr_vec;
    for(auto it : processes) pr_vec.push_back(it.first);
    while(pr_vec.size() > 0) {
        int ticket = generateTicket();
        int pid = -1;
        for(int i=0; i<pr_vec.size(); i++) {
            int start = processes[pr_vec[i]].startShare;
            int end = processes[pr_vec[i]].endShare;
            if(ticket >= start && ticket <= end) {
                pid = pr_vec[i];
                break;
            }
        }
        if(pid == -1) continue;
        if(index.count(pid) == 0) index[pid] = 0;
        else index[pid]++;

        int arrival = mp[pid].arrival;
        int i = index[pid];

        if(lastTime==-1) lastTime = arrival;
        if(i >= mp[pid].burst.size()) {
            time[pid].push_back(lastTime);
            vector<int> pr_vec2;
            for(auto it : pr_vec) {
                if(it != pid) pr_vec2.push_back(it);
            }
            pr_vec = pr_vec2;
        } else if(mp[pid].burst[i].first == "P") {
            time[pid].push_back(lastTime);
            lastTime += mp[pid].burst[i].second;
            time[pid].push_back(lastTime);
            arrival = lastTime;
        } else {
            arrival += mp[pid].burst[i].second;
        }
    }

    cout << endl << endl;
    cout <<"Priority (Non-Preemptive):" << endl;
    print(time);
    cout << endl << endl;
}

void priorityPreEmptive() {
    queue<pair<pair<int, int>, pair<int, int>>> pq;
    for(auto it : processes) {
        int pid = it.first;
        int arrival = it.second.arrival;
        int priority = it.second.priority;
        pq.push({{arrival, priority}, {pid, 0}});
    }
    compSort(pq, -1);
    unordered_map<int, Process> mp = processes;
    unordered_map<int, vector<int>> time;
    int lastTime = -1;
    while(!pq.empty()) {
        int arrival = pq.front().first.first;
        int priority = pq.front().first.second;
        int pid = pq.front().second.first;
        int i = pq.front().second.second;
        pq.pop();

        if(lastTime == -1) lastTime = arrival;
        if(i >= mp[pid].burst.size()) {
            time[pid].push_back(lastTime);
        } else if(mp[pid].burst[i].first == "P") {
            time[pid].push_back(lastTime);
            lastTime += 1;
            mp[pid].burst[i].second -= 1;
            time[pid].push_back(lastTime);
            arrival = lastTime;
            if(mp[pid].burst[i].second == 0) pq.push({{arrival, priority},{pid, i + 1}});
            else pq.push({{arrival, priority}, {pid, i}});
        } else {
            arrival += mp[pid].burst[i].second;
            pq.push({{arrival, priority}, {pid, i + 1}});
        }
        compSort(pq,lastTime);
    }
    cout << endl << endl;
    cout <<"Priority (Pre-emptive):" << endl;
    print(time);
    cout << endl << endl;
}

int main(){
    freopen("process.dat", "r", stdin);
    freopen("output.txt", "w", stdout);

    cin >> num_processes >> quantum >> lotteryTickets;
    for(int i=0; i<num_processes; i++){
        int pid;
        cin >> pid;
        int arrival, priority, share1, share2;
        cin >> arrival >> priority >> share1 >> share2;
        processes[pid].arrival = arrival;
        processes[pid].priority = priority;
        processes[pid].startShare = share1;
        processes[pid].endShare = share2;
        while(true) {
            string x;
            cin >> x;
            if(x == "-1") break;
            int b;
            cin >> b;
            processes[pid].burst.push_back({x, b});
        }
        process_queue.push({pid, processes[pid]});
    }

    priorityPreEmptive();
    priorityNonPreEmptive();
    roundRobin();
}

/*
Sample Input
5 3 32
1 0 25 0 5 P 2 I 3 P 5 -1
2 2 20 6 15 P 4 I 5 P 4 -1
3 1 26 15 16 P 3 I 2 P 3 -1
4 3 19 17 30 P 2 I 5 P 5 -1
5 2 30 31 31 P 5 I 1 P 4 -1
*/