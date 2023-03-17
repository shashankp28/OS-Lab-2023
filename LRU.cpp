#include <iostream>
#include <bits/stdc++.h>
#include <fstream>

using namespace std;

void read_file(vector<int> &requests, string input_file)
{
    ifstream infile(input_file);
    int num;
    while (infile >> num)
    {
        requests.push_back(num);
    }
    infile.close();
}
int get_lru_request(unordered_map<int, int> &memory_frames){
    int min_time = INT_MAX, min_req=0;
    for(auto kv : memory_frames){
        if(min_time > kv.second){
            min_time = kv.second;
            min_req = kv.first;
        }
    }
    return min_req;
}

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        cout << "Usage: ./FIFO <addressable_size> <memory_size> <disk_size> <input_file>" << endl;
        return 0;
    }

    int addressable_size = atoi(argv[1]);
    int memory_size = atoi(argv[2]);
    int disk_size = atoi(argv[3]);
    string input_file(argv[4]);
    int pageFaults = 0;

    vector<int> requests;
    unordered_map<int, int> memory_frames;
    read_file(requests);

    for (int i = 0; i < requests.size(); i++)
    {
        if (memory_frames.find(requests[i]) != memory_frames.end()){
            memory_frames[requests[i]] = i;
            continue;
        }

        pageFaults++;
        if (memory_frames.size() >= memory_size)
        {
            int swap = get_lru_request(memory_frames);
            memory_frames.erase(swap);
        }
        memory_frames[requests[i]] = i;
    }

    cout << "Page Faults " << pageFaults << "\n";
    cout << "Total " << requests.size() << "\n";

    return 0;
}