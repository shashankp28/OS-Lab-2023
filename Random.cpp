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

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        cout << "Usage: ./FIFO <addressable_size> <memory_size> <disk_size> <input_file>" << endl;
        return 0;
    }

    cout << "--------------------------------------------" << endl;
    cout << "Random" << endl;

    int addressable_size = atoi(argv[1]);
    int memory_size = atoi(argv[2]);
    int swap_size = atoi(argv[3]);
    string input_file(argv[4]);
    unordered_set<int> swap_space;

    int pageFaults = 0;
    srand(time(NULL));

    vector<int> requests;
    read_file(requests, input_file);
    unordered_set<int> unique_requests(requests.begin(), requests.end());
    cout << "Unique Requests: " << unique_requests.size() << endl;

    unordered_set<int> memory_frames;

    for (auto i : requests)
    {

        if (memory_frames.find(i) != memory_frames.end())
            continue;

        pageFaults++;
        if (memory_frames.size() >= memory_size)
        {
            int index = rand() % memory_size;
            auto itr = memory_frames.begin();
            advance(itr, index);
            memory_frames.erase(itr);
            swap_space.insert(*itr);
            if (swap_space.size() > swap_size)
            {
                cout << "Error: Swap space is full\n";
                return 1;
            }
        }
        if (swap_space.find(i) != swap_space.end())
            swap_space.erase(i);
        memory_frames.insert(i);
    }

    cout << "Page Faults: " << pageFaults << "\n";
    cout << "Total: " << requests.size() << "\n";

    return 0;
}