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

    int addressable_size = atoi(argv[1]);
    int memory_size = atoi(argv[2]);
    int disk_size = atoi(argv[3]);
    string input_file(argv[4]);
    int pageFaults = 0;
    srand(time(NULL));

    vector<int> requests;
    read_file(requests);

    unordered_set<int> memory_frames;

    int ind = 0;
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
        }
        memory_frames.insert(i);
    }

    cout << "Page Faults " << pageFaults << "\n";
    cout << "Total " << requests.size() << "\n";

    return 0;
}