#include <iostream>
#include <vector>
#include <queue>
using namespace std;

const int NUM_PAGES = 15;
const int NUM_FRAMES = 4;

// 0 1 2 3 2 5 7 1 9 5 9 1 5 2 12
int main() {
    vector<int> page(NUM_PAGES);
    cout << "Enter sequence of pages" << endl;
    for(int i=0; i<NUM_PAGES; i++) {
        cin >> page[i];
    }
    
    queue<int> page_queue;
    int faults = 0;

    for (int i = 0; i < NUM_PAGES; i++) {
        bool page_found = false;
        queue<int> tmp_queue;
        while(!page_queue.empty()) {
            if (page_queue.front() == page[i]) {
                page_found = true;
                break;
            }
            tmp_queue.push(page_queue.front());
            page_queue.pop();
        }
        while(!page_queue.empty()) {
            tmp_queue.push(page_queue.front());
            page_queue.pop();
        }
        while(!tmp_queue.empty()) {
            page_queue.push(tmp_queue.front());
            tmp_queue.pop();
        }

        if (!page_found) {
            if (page_queue.size() == NUM_FRAMES) {
                page_queue.pop();
            }
            page_queue.push(page[i]);
            faults++;
            cout << page[i] << " : " << "MISS" << endl;
        } else {
            cout << page[i] << " : " << "HIT" << endl;
        }

        cout << "\tFrames : ";
        for (int j = 0; j < page_queue.size(); j++) {
            cout << page_queue.front() << " ";
            page_queue.push(page_queue.front());
            page_queue.pop();
        }
        cout << endl;
    }

    cout << "Hit ratio: " << (((double)(NUM_PAGES - faults) / NUM_PAGES) * 100) << endl;

    return 0;
}
