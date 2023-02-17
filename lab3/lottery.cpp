#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <numeric>

using namespace std;

struct Process {
    int pid;
    int arrival_time;
    int priority;
    int share;
    vector<int> cpu_bursts;
    vector<int> io_bursts;
    int remaining_time;
    int waiting_time;
    int response_time;
    int turnaround_time;
    int tickets;
    int completed_bursts;
    int current_burst;
    int last_executed;
    Process(int p, int a, int pr, int sh, vector<int> cb, vector<int> ib) {
        pid = p;
        arrival_time = a;
        priority = pr;
        share = sh;
        cpu_bursts = cb;
        io_bursts = ib;
        remaining_time = 0;
        waiting_time = 0;
        response_time = 0;
        turnaround_time = 0;
        tickets = 0;
        completed_bursts = 0;
        current_burst = 0;
        last_executed = -1;
        for (auto b : cpu_bursts) {
            remaining_time += b;
        }
    }
};

void lottery_scheduling(vector<Process> processes) {
    // Set seed for the random number generator based on the time of day
    srand(time(NULL));

    // Calculate the total number of tickets available
    int total_tickets = 0;
    for (auto& p : processes) {
        p.tickets = p.share;
        total_tickets += p.tickets;
    }

    int current_time = 0;
    vector<int> completed_pids;

    // Run the lottery scheduling algorithm until all processes are completed
    while (completed_pids.size() < processes.size()) {
        int winning_ticket = rand() % total_tickets;
        int winning_ind = -1;
        int ticket_sum = 0;

        // Determine which process won the lottery based on its number of tickets
        for (int i=0; i<processes.size(); i++) {
            auto &p = processes[i];
            ticket_sum += p.tickets;
            if (ticket_sum > winning_ticket) {
                winning_ind = i;
                break;
            }
        }

        // Get the winning process from the list of processes
        auto& p = processes[winning_ind];

        // If this is the first time the process is being executed, update its response time
        if (p.last_executed == -1) {
            p.response_time = current_time - p.arrival_time;
        }

        // Execute the process for one time quantum
        int remaining_burst = p.cpu_bursts[p.current_burst];
        if (remaining_burst > p.remaining_time) {
            remaining_burst = p.remaining_time;
        }
        p.remaining_time -= remaining_burst;

        // Update waiting time for all other processes
        for (auto& other_p : processes) {
            if (other_p.pid != p.pid && other_p.remaining_time > 0) {
                other_p.waiting_time += remaining_burst;
            }
        }

        // Update the current burst of the process
        p.completed_bursts++;
        if (p.completed_bursts == p.cpu_bursts.size()) {
            // The process has completed all of its bursts
            completed_pids.push_back(p.pid);
            p.turnaround_time = current_time + remaining_burst - p.arrival_time;
        } else {
            // The process still has more bursts to execute
            p.last_executed = current_time;
            p.current_burst++;
        }

        // Update the total time
        current_time += remaining_burst;

        // Reset the number of tickets for this process
        p.tickets = p.share;

        // Recalculate the total number of tickets
        total_tickets = 0;
        for (auto& other_p : processes) {
            if (other_p.remaining_time > 0) {
                total_tickets += other_p.tickets;
            }
        }
    }

    // Calculate statistics for each process
    vector<int> turnaround_times;
    vector<int> waiting_times;
    vector<int> response_times;
    int total_turnaround = 0, total_waiting = 0, total_response = 0;
    for (auto& p : processes) {
        turnaround_times.push_back(p.turnaround_time);
        waiting_times.push_back(p.waiting_time);
        response_times.push_back(p.response_time);
        total_turnaround += p.turnaround_time;
        total_waiting += p.waiting_time;
        total_response += p.response_time;
    }

    // Print out the statistics
    cout << "\tTurnaround Time: " << endl;
    for (auto& p : processes) {
        cout << "\t\tProcess " << p.pid << ": " << p.turnaround_time << endl;
    }
    cout << "\t\tTotal: " << total_turnaround << endl;
    cout << "\t\tAverage: " << total_turnaround / (double)turnaround_times.size() << endl;

    cout << "\tWaiting Time: " << endl;
    for (auto& p : processes) {
        cout << "\t\tProcess " << p.pid << ": " << p.waiting_time << endl;
    }
    cout << "\t\tTotal: " << total_waiting << endl;
    cout << "\t\tAverage: " << total_waiting / (double)waiting_times.size() << endl;

    cout << "\tResponse Time: " << endl;
    for (auto& p : processes) {
        cout << "\t\tProcess " << p.pid << ": " << p.response_time << endl;
    }
    cout << "\t\tTotal: " << total_response << endl;
    cout << "\t\tAverage: " << total_response / (double)response_times.size() << endl;
}

int main() {
    freopen("process.dat", "r", stdin);
    freopen("output.txt", "w", stdout);
    
    int num_processes, quantum, num_tickets;
    cin >> num_processes >> quantum >> num_tickets;
    vector<Process> processes;
    for (int i = 0; i < num_processes; i++) {
        int pid, arrival_time, priority, share;
        cin >> pid >> arrival_time >> priority >> share;
        vector<int> cpu_bursts, io_bursts;
        int burst;
        cin >> burst;
        while (burst != -1) {
            cpu_bursts.push_back(burst);
            cin >> burst;
            io_bursts.push_back(burst);
            cin >> burst;
        }
        processes.push_back(Process(pid, arrival_time, priority, share, cpu_bursts, io_bursts));
    }

    cout << "Lottery:" << endl;
    lottery_scheduling(processes);
    return 0;
}

/*
Sample Input:
3
20
10
1 0 0 2 20 35 15 50 -1
2 10 1 4 30 60 20 80 -1
3 30 2 4 25 40 30 60 -1
*/