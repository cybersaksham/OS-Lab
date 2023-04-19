#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const int MAX_RESOURCES = 100;
const int MAX_PROCESSES = 100;

struct Process {
    int id;
    vector<int> resource_req;
    vector<int> allocated;
};

struct Resource {
    int id;
    int num_instances;
};

pair<bool, queue<int>> hasDeadlock(vector<Resource>& resources, vector<Process>& processes) {
    vector<int> available(resources.size(), 0);

    vector<vector<int>> need(processes.size(), vector<int>(resources.size(), 0));
    for (int i = 0; i < processes.size(); i++) {
        for (int j = 0; j < resources.size(); j++) {
            need[i][j] = processes[i].resource_req[j] - processes[i].allocated[j];
            available[j] += processes[i].allocated[j];
        }
    }

    queue<int> completed;

    vector<int> blocked(processes.size(), 0);

    int num_completed = 0;

    while (num_completed < processes.size()) {
        int i;
        for (i = 0; i < processes.size(); i++) {
            if (blocked[i] == 0) {
                bool can_complete = true;
                for (int j = 0; j < resources.size(); j++) {
                    if (need[i][j] > available[j]) {
                        can_complete = false;
                        break;
                    }
                }
                if (can_complete) {
                    completed.push(processes[i].id);
                    blocked[i] = -1;
                    num_completed++;
                    for (int j = 0; j < resources.size(); j++) {
                        available[j] += processes[i].allocated[j];
                    }
                    break;
                }
            }
        }

        if (i == processes.size()) {
            return {true, completed};
        }
    }

    return {false, completed};
}

int main() {
    freopen("input.dat", "r", stdin);
    int num_resources;
    cin >> num_resources;

    vector<Resource> resources(num_resources);
    for (int i = 0; i < num_resources; i++) {
        cin >> resources[i].id >> resources[i].num_instances;
    }

    int num_processes;
    cin >> num_processes;

    vector<Process> processes(num_processes);
    for (int i = 0; i < num_processes; i++) {
        int id;
        cin >> id;
        processes[i].id = id;
        processes[i].resource_req.resize(num_resources);
        processes[i].allocated.resize(num_resources);

        for (int j = 0; j < num_resources; j++) {
            int req, alloc;
            cin >> req >> alloc;
            processes[i].resource_req[j] = req;
            processes[i].allocated[j] = alloc;
        }
    }

    pair<bool, queue<int>> has_deadlock = hasDeadlock(resources, processes);

    if (has_deadlock.first) {
        cout << "Deadlock detected" << endl;
    } else {
        cout << "No deadlock detected" << endl;
        cout << "Process execution sequence: ";
        queue<int> q = has_deadlock.second;
        while(!q.empty()) {
            cout << q.front() << " ";
            q.pop();
        }
        cout << endl;
    }

    return 0;
}