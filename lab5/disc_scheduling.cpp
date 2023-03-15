#include <bits/stdc++.h>
using namespace std;

int main() {
  int cylinders, sectors, bps, rpm, avg_seek_time, initial_head;
  fstream input_file("input.txt", ios::in);
  
  input_file >> cylinders >> sectors >> bps >> rpm >> avg_seek_time >> initial_head;

  vector<int> requests;
  while(!input_file.eof()) {
    int a;
    input_file >> a;
    requests.push_back(a);
  }
  int n_request = requests.size();

  // FCFS
  float avg_rotation_fcfs = (sectors * 60.0) / (rpm * 2);
  int total_seek_fcfs = 0;
  for(int i=0; i<n_request; i++) {
    if(i == 0) total_seek_fcfs += abs(requests[i] - initial_head) * avg_seek_time;
    else total_seek_fcfs += abs(requests[i] - requests[i-1]) * avg_seek_time;
  }

  // C-SCAN
  sort(requests.begin(), requests.end());

  float avg_rotational_cscan = (sectors * 60.0) / (rpm * 2);

  int total_seek_cscan = 0;
  if(initial_head >= requests[n_request-1]) {
    total_seek_cscan += abs(initial_head - 0) * avg_seek_time;
    total_seek_cscan += abs(requests[n_request-1] - 0) * avg_seek_time;
  } else if(initial_head <= requests[0]) {
    total_seek_cscan += abs(initial_head - requests[n_request-1]) * avg_seek_time;
  } else {
    // First Right
    int i = 0;
    while(i<n_request && requests[i]<initial_head) i++;
    int right = 0;
    right += abs(initial_head - (cylinders-1));
    right += abs(0 - (cylinders-1));
    right += abs(requests[i-1] - 0);

    // First Left
    int left = 0;
    left += abs(initial_head - 0);
    left += abs(requests[n_request-1] - 0);

    total_seek_cscan += min(left, right) * avg_seek_time;
  }

  cout << "FCFS Average Rotational Delay: " << avg_rotation_fcfs << endl;
  cout << "FCFS Total Seek Time: " << total_seek_fcfs << endl;
  cout << "C-SCAN Average Rotational Delay: " << avg_rotational_cscan << endl;
  cout << "C-SCAN Total Seek Time: " << total_seek_cscan << endl;

  return 0;
}
