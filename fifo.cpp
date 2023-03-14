#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <cstdlib>
#include <ctime>

using namespace std;

int page_faults = 0;
int hits = 0;

// Function to simulate page fault handling
void fifoEvict(int page, unordered_map<int, int>& page_table, vector<int>& frames, int& next_frame, int num_frames,
                       vector<bool>& frame_in_use, vector<int>& swap_space, queue<int>& loaded_frames)
{
    // Memory is full, use FIFO policy to select frame to swap out
    int victim_frame = loaded_frames.front();
    loaded_frames.pop();
    int victim_page = frames[victim_frame];
    frames[victim_frame] = page;
    page_table.erase(victim_page);
    page_table[page] = victim_frame;
    swap_space[victim_page] = victim_frame;
    loaded_frames.push(victim_frame);
}

int main(int argc, char* argv[]) {

    if(argc != 5) {
        cerr << "Usage: " << argv[0] << " <# logical pages> <# physical frames> <# swap blocks> <page requests file>" << endl;
        return 1;
    }

    int num_pages = atoi(argv[1]);
    int num_frames = atoi(argv[2]);
    int num_blocks = atoi(argv[3]);
    string page_requests = argv[4];
    
    // Initialize data structures
    unordered_map<int, int> page_table;
    vector<int> frames(num_frames, -1);
    vector<bool> frame_in_use(num_frames, false);
    vector<int> swap_space(num_pages, -1);
    int next_frame = 0;
    
    // Read page requests from input file
    ifstream infile(page_requests);
    int page;
    queue<int> loaded_frames;
    while (infile >> page) 
    {
        if (page_table.find(page) != page_table.end()) {

            // Page found in memory
            hits++;
            continue;
        }

        page_faults++;
        if (next_frame < num_frames) 
        {
            // Memory is not full, add page to next available frame
            frames[next_frame] = page;
            page_table[page] = next_frame;
            frame_in_use[next_frame] = true;
            loaded_frames.push(next_frame); // Add the newly loaded frame to the back of the queue
            next_frame++;
        } 
        else 
        {
            fifoEvict(page, page_table, frames, next_frame, num_frames,
                          frame_in_use, swap_space, loaded_frames);
        }
    }
    
    // Print page table
    // for (auto it = page_table.begin(); it != page_table.end(); ++it) 
    // {
    //     cout << "Page " << it->first << " is in frame " << it->second << endl;
    // }

    cout << "--------------------------------------------------------" << endl;
    cout << "FIFO POLICY" << endl;
    cout << "Number of hits: " << hits << endl;
    cout << "Number of page faults: " << page_faults << endl;
    
    return 0;
}