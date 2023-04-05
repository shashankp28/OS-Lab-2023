#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <sstream>
#include <unordered_map>

using namespace std;

// Global variables
int timestamp = 0, min_split = 0; // timestamp keeps track of when a process was last freed, min_split is the smallest block size that can be split

// BuddyMemoryManager class declaration
class BuddyMemoryManager
{

private:
    string node_name;                                          // the name of the node: "Free Block", "Internal Node", or a process ID
    int node_size;                                             // the size of the node
    int last_freed;                                            // the timestamp of when a process was last freed from this node
    BuddyMemoryManager *left;                                  // the left child of this node in the binary tree
    BuddyMemoryManager *right;                                 // the right child of this node in the binary tree
    int allotted_size;                                         // the size of the node that has been allotted to a process (if the node is a process node)
    unordered_map<char, BuddyMemoryManager *> process_to_node; // a map from process IDs to the node that the process is stored in

public:
    BuddyMemoryManager(string node_n, int node_s) // constructor
    {
        node_name = node_n;
        node_size = node_s;
        allotted_size = 0;
        last_freed = timestamp++; // initialize last_freed to the current timestamp, then increment it
        left = NULL;
        right = NULL;
    }

    BuddyMemoryManager *find_best_fit(int size, int &current_best_size, BuddyMemoryManager *best_fit) // helper function to find the best-fit node to allocate a process
    {
        // if this node is a free block and has no children
        if (node_name == "Free Block" && !left && !right) 
        {
            // if there is no best-fit node yet and this node is large enough to fit the process
            if (!best_fit && node_size >= size) 
            {
                best_fit = this;
                current_best_size = node_size;
            }

            // if this node is smaller than the current best-fit node but still large enough to fit the process
            else if (node_size < current_best_size && node_size >= size)             
            {
                best_fit = this;
                current_best_size = node_size;
            }
            
            else if (node_size == current_best_size && node_size >= size) // if this node is the same size as the current best-fit node and still large enough to fit the process
            {
                if (last_freed > best_fit->last_freed) // choose the node that was last freed
                {
                    best_fit = this;
                    current_best_size = node_size;
                }
            }
            return best_fit; // return the best-fit node (may be NULL)
        }
        if (left != NULL) // if this node has a left child, recursively call find_best_fit on it
        {
            best_fit = left->find_best_fit(size, current_best_size, best_fit);
        }
        if (right != NULL) // if this node has a right child, recursively call find_best_fit on it
        {
            best_fit = right->find_best_fit(size, current_best_size, best_fit);
        }
        return best_fit; // return the best-fit node (may be NULL)
    }

    int alloc(int size, char pid)
    {
        int best_fit_value = 0;
        // Find the best fitting free block for the requested size
        BuddyMemoryManager *best_fit = find_best_fit(size, best_fit_value, NULL);

        // If no block is available, return 0
        if (!best_fit)
        {
            cout << "Not enough memory" << endl;
            return 0;
        }
        else
        {
            // Split the block until it is of the desired size or it cannot be split further
            while ((best_fit->node_size) / 2 >= size && (best_fit->node_size) / 2 > min_split)
            {
                // Create left and right child nodes of the current block
                best_fit->left = new BuddyMemoryManager("Free Block", best_fit->node_size / 2);
                best_fit->right = new BuddyMemoryManager("Free Block", best_fit->node_size / 2);
                best_fit->node_name = "Internal Node";
                best_fit = best_fit->left; // Set the current block to be the left child
            }

            // Set the node name to the process ID, and record the allotted size and timestamp
            best_fit->node_name = pid;
            best_fit->allotted_size = size;
            best_fit->last_freed = 0;

            // Map the process ID to the allocated node
            process_to_node[pid] = best_fit;
        }
        return 1;
    }

    int dealloc(char name)
    {
        // Check if the process ID is in the mapping
        if (process_to_node.find(name) != process_to_node.end())
        {
            // Get the allocated node for the process
            BuddyMemoryManager *node = process_to_node[name];

            // Reset the node's properties to those of a free block, and record the timestamp
            node->node_name = "Free Block";
            node->allotted_size = 0;
            node->last_freed = timestamp++;

            // Remove the mapping for the process ID
            process_to_node.erase(name);
        }
        else
        {
            cout << "Process not found" << endl;
            return 0;
        }

        // Merge adjacent free blocks
        merge();
        return 1;
    }

    void merge()
    {
        // If the current node has left and right children
        if (left && right)
        {
            // Recursively call merge on both children
            left->merge();
            right->merge();

            // If both children are free blocks, merge them and update the node name
            if (left->node_name == "Free Block" && right->node_name == "Free Block")
            {
                left = NULL;
                right = NULL;
                node_name = "Free Block";
            }
        }
        return;
    }

    void print()
    {
        if (left && right)
        {
            left->print();
            right->print();
        }
        if (node_name == "Free Block")
        {
            cout << node_name << ": " << node_size << endl;
        }
        else if(node_name != "Internal Node")
        {
            cout << node_name << ": " << allotted_size << endl;
        }
        return;
    }
};

int main(int argc, char** argv)
{

    if(argc != 2)
    {
        cout<<"Usage: ./BMM <input_file_path>"<<endl;
        return 1;
    }

    ifstream file(argv[1]);
    string line;
    int test_cases;

    if (file.is_open())
    {
        getline(file, line);
        test_cases = stoi(line);
        getline(file, line);

        for (int t = 0; t < test_cases; t++)
        {
            getline(file, line);
            int U, L;
            std::stringstream ss(line);
            ss >> U >> L;
            getline(file, line);

            U = (int)pow(2, U);
            L = (int)pow(2, L);

            BuddyMemoryManager *root = new BuddyMemoryManager("Free Block", U);

            getline(file, line);
            while (line != "")
            {

                char id;
                int size;

                std::stringstream ss(line);
                ss >> id >> size;

                if (size)
                {
                    if(!root->alloc(size, id))
                    {
                        return 1;
                    }
                }
                else
                {
                    if(!root->dealloc(id))
                    {
                        return 1;
                    }
                }
                if (file.fail() || file.eof())
                {
                    break;
                }
                getline(file, line);
            }

            root->print();
            if(t != test_cases-1) cout << endl;
        }
        file.close();
    }
    else
    {
        cerr << "Unable to open file" << endl;
    }

    return 0;
}