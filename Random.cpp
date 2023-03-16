#include <iostream>
#include <bits/stdc++.h>
#include <fstream>

using namespace std;

void read_file(vector<int> &requests)
{
    ifstream infile("req1.dat");
    int num;
    while (infile >> num)
    {
        requests.push_back(num);
    }
    infile.close();
}

int main()
{
    int addressable_size = 60;
    int memory_size = 20;
    int disk_size = 60;
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