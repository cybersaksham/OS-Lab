#include <bits/stdc++.h>
using namespace std;

class Process {
    public:
        int pid;
        string name;
        vector<Process *> children;
        Process *parent;

        void add_child(Process *child) {
            children.push_back(child);
            child->parent = this;
        }

        Process(int pid, string name) {
            this->pid = pid;
            this->name = name;
            this->parent = NULL;
        }

        ~Process() {
            for(auto child : children) delete child;
            if(parent) {
                vector<Process *>::iterator it = parent->children.begin();
                for(auto child : parent->children) {
                    if(child->pid == pid) break;
                    it++;
                }
                parent->children.erase(it);
            }
        }
};

void print_tree(Process *root, int depth) {
    if (root == NULL) return;

    for (int i = 0; i < depth; i++) cout << "\t";
    cout << "|_" << root->name << "(" << root->pid << ")" << endl;
    for (int i = 0; i < root->children.size(); i++) {
        print_tree(root->children[i], depth + 1);
    }
}

int main() {
    Process *root = new Process(1, "init");

    int pids = 1;
    map<int, Process *> process_map;
    process_map[1] = root;

    while (true) {
        cout << endl << endl;
        cout << "Enter 1 to generate new process." << endl;
        cout << "Enter 2 to terminate a process." << endl;
        int query = 0;
        cout << "Query: ";
        cin >> query;

        if(query == 1) {
            int pid = pids + 1;
            Process *p = new Process(pid, "Process_" + to_string(pid));
            cout << "Enter pid of its parent. 1 for root: ";
            int ppid = 1;
            cin >> ppid;
            if (process_map.find(ppid) == process_map.end()) {
                cout << "Process ID does not exist. Making root as parent." << endl;
                ppid = 1;
            }
            process_map[pid] = p;
            Process *parent = process_map[ppid];
            parent->add_child(p);
            pids++;
        } else if(query == 2) {
            int pid = 0;
            cout << "Enter pid to terminate: ";
            cin >> pid;
            if (process_map.find(pid) == process_map.end()) {
                cout << "Process ID does not exist. Cannot terminate" << endl;
                pid = 0;
            }
            if(pid == 1) {
                cout << "Terminating main process..." << endl;
                delete root;
                exit(1);
            }
            if(pid != 0) {
                Process *p = process_map[pid];
                delete p;
                process_map.erase(pid);
            }
        }

        system("clear");
        print_tree(root, 0);
    }

    return 0;
}
