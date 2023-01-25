#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

struct Process {
    int pid;
    int priority;
    int arrivalTime;
    vector<pair<char, int>> bursts;
	int turnaroundTime;
	int responseTime;
	int waitingTime;
	int totalBurst;
};

void readData(string fileName, int &numProcesses, int &quantum, vector<Process> &processes) {
    ifstream inputFile(fileName);

    if (inputFile.is_open()) {
        inputFile >> numProcesses >> quantum;

        for (int i = 0; i < numProcesses; i++) {
            int pid, priority, arrivalTime;
            inputFile >> pid >> priority >> arrivalTime;

            Process p;
            p.pid = pid;
            p.priority = priority;
            p.arrivalTime = arrivalTime;

            char device;
            int burst;
            while (inputFile >> device && device != '-') {
                inputFile >> burst;
                p.bursts.push_back({ device, burst });
            }

            processes.push_back(p);
			inputFile >> device;
        }

        inputFile.close();
    }
}

void FCFS(vector<Process> processes) {
    cout << "FCFS Scheduling:" << endl;

    // Sort processes based on arrival time
    sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int currentTime = 0;
    vector<int> turnaroundTime, responseTime, waitingTime, burstTime;

    for (Process p : processes) {
        int pid = p.pid;
        int arrivalTime = p.arrivalTime;

        if (arrivalTime > currentTime) {
            currentTime = arrivalTime;
        }

        int response = currentTime - arrivalTime;
        responseTime.push_back(response);

        int totalBurst = 0;
        for (auto burst : p.bursts) {
            totalBurst += burst.second;
        }

        int turnaround = totalBurst + response;
        turnaroundTime.push_back(turnaround);
        waitingTime.push_back(response);
        burstTime.push_back(totalBurst);

        currentTime += totalBurst;
    }

    // Print results
    cout << "Process\tTurnaround-Time\tResponse-Time\tWaiting-Time\tBurst-time" << endl;
    for (int i = 0; i < processes.size(); i++) {
        cout << processes[i].pid << "\t\t" << turnaroundTime[i] << "\t\t" << responseTime[i] << "\t\t" << waitingTime[i] << "\t\t" << burstTime[i] << endl;
    }

    // Calculate average turnaround time, response time, and waiting time
    int totalTurnaround = 0, totalResponse = 0, totalWaiting = 0;
    for (int i = 0; i < processes.size(); i++) {
        totalTurnaround += turnaroundTime[i];
        totalResponse += responseTime[i];
        totalWaiting += waitingTime[i];
    }

    cout << "Average Turnaround Time: " << totalTurnaround / (double)processes.size() << endl;
    cout << "Average Response Time: " << totalResponse / (double)processes.size() << endl;
    cout << "Average Waiting Time: " << totalWaiting / (double)processes.size() << endl;
}

void SJF(vector<Process> processes) {
    cout << "SJF Scheduling:" << endl;

    vector<Process> processesCopy = processes;

    // Sort processes based on burst time
    sort(processesCopy.begin(), processesCopy.end(), [](Process a, Process b) {
        int totalBurstA = 0, totalBurstB = 0;
        for (auto burst : a.bursts) {
            totalBurstA += burst.second;
        }
        for (auto burst : b.bursts) {
            totalBurstB += burst.second;
        }
        return totalBurstA < totalBurstB;
    });

    int currentTime = 0;
    vector<int> turnaroundTime, responseTime, waitingTime, burstTime;

    for (Process p : processesCopy) {
        int pid = p.pid;
        int arrivalTime = p.arrivalTime;

        if (arrivalTime > currentTime) {
            currentTime = arrivalTime;
        }

        int response = currentTime - arrivalTime;
        responseTime.push_back(response);

        int totalBurst = 0;
        for (auto burst : p.bursts) {
            totalBurst += burst.second;
        }

        int turnaround = totalBurst + response;
        turnaroundTime.push_back(turnaround);
        waitingTime.push_back(response);
        burstTime.push_back(totalBurst);

        currentTime += totalBurst;
    }

    // Print results
    cout << "Process\tTurnaround-Time\tResponse-Time\tWaiting-Time\tBurst-time" << endl;
    for (int i = 0; i < processesCopy.size(); i++) {
        cout << processesCopy[i].pid << "\t\t" << turnaroundTime[i] << "\t\t" << responseTime[i] << "\t\t" << waitingTime[i] << "\t\t" << burstTime[i] << endl;
    }

    // Calculate average turnaround time, response time, and waiting time
    int totalTurnaround = 0, totalResponse = 0, totalWaiting = 0;
    for (int i = 0; i < processesCopy.size(); i++) {
        totalTurnaround += turnaroundTime[i];
        totalResponse += responseTime[i];
        totalWaiting += waitingTime[i];
    }

    cout << "Average Turnaround Time: " << totalTurnaround / (double)processesCopy.size() << endl;
    cout << "Average Response Time: " << totalResponse / (double)processesCopy.size() << endl;
    cout << "Average Waiting Time: " << totalWaiting / (double)processesCopy.size() << endl;
}


int main() {
    int numProcesses, quantum;
    vector<Process> processes;
    readData("input.dat", numProcesses, quantum, processes);

	cout << endl;
    FCFS(processes);
	cout << endl;
    SJF(processes);
	cout << endl;

    return 0;
}

