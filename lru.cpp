#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <queue>

using namespace std;

int page_faults = 0;
int hits = 0;

// Function to simulate page fault handling
void lruEvict(int page, unordered_map<int, int>& page_table, vector<int>& page_queue, int num_frames,
              vector<bool>& frame_in_use, vector<int>& swap_space) 
{
    int victim_page = page_queue[0];
    page_queue.erase(page_queue.begin());
    
    int victim_frame = page_table[victim_page];
    page_table.erase(victim_page);
    
    // Replace the victim page with the new page in the frame
    page_table[page] = victim_frame;
    swap_space[victim_page] = -1;
    swap_space[page] = victim_frame;
    
    // Update the frame usage and page queue
    frame_in_use[victim_frame] = true;
    page_queue.push_back(page);
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
    
    if (num_blocks <= num_pages) {
        cerr << "Error: Swap size is less than the number of blocks required." << endl;
        exit(1);
    }

    // Initialize data structures
    unordered_map<int, int> page_table;
    vector<int> page_queue;
    vector<bool> frame_in_use(num_frames, false);
    vector<int> swap_space(num_pages, -1);
    int next_frame = 0;
    
    // Read page requests from input file
    ifstream infile(page_requests);
    int page;
    while (infile >> page) 
    {
        if (page_table.find(page) != page_table.end()) 
        {
            // Page found in memory, update the page queue
            for (auto it = page_queue.begin(); it != page_queue.end(); ++it) 
            {
                if (*it == page) {
                    page_queue.erase(it);
                    break;
                }
            }
            page_queue.push_back(page);
            hits++;
            continue;
        }
        
        page_faults++;
        if (next_frame < num_frames) 
        {
            // Memory is not full, add page to next available frame
            page_table[page] = next_frame;
            frame_in_use[next_frame] = true;
            swap_space[page] = next_frame;
            page_queue.push_back(page);
            next_frame++;
        } 
        else 
        {
            lruEvict(page, page_table, page_queue, num_frames, frame_in_use, swap_space);
        }
    }
    
    // Print page table
    // for (auto it = page_table.begin(); it != page_table.end(); ++it) {
    //     cout << "Page " << it->first << " is in frame " << it->second << endl;
    // }

    // cout << "--------------------------------------------------------" << endl;
    // cout << "LRU POLICY USING QUEUE" << endl;
    // cout << "Number of hits: " << hits << endl;
    // cout << "Number of page faults: " << page_faults << endl;
    cout << "End";
    //cout << page_faults << endl;
    
    return 0;
}
