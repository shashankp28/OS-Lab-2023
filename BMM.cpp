#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <sstream>
#include <bits/stdc++.h>

using namespace std;

int timestamp = 0, min_split = 0;

class BuddyMemoryManager
{

private:
    string node_name;
    int node_size;
    int last_freed;
    BuddyMemoryManager *left;
    BuddyMemoryManager *right;
    int allotted_size;
    unordered_map<char, BuddyMemoryManager *> process_to_node;

public:
    BuddyMemoryManager(string node_n, int node_s)
    {
        node_name = node_n;
        node_size = node_s;
        allotted_size = 0;
        last_freed = timestamp++;
        left = NULL;
        right = NULL;
    }

    BuddyMemoryManager* find_best_fit(int size, int &current_best_size, BuddyMemoryManager* best_fit)
    {
        if (node_name == "Free Block" && !left && !right)
        {
            if (!best_fit && node_size >= size)
            {
                best_fit = this;
                current_best_size = node_size;
            }
            else if (node_size < current_best_size && node_size >= size)
            {
                best_fit = this;
                current_best_size = node_size;
            }
            else if (node_size == current_best_size && node_size >= size)
            {
                if (last_freed > best_fit->last_freed)
                {
                    best_fit = this;
                    current_best_size = node_size;
                }
            }
            return best_fit;
        }
        if (left != NULL)
        {
            best_fit = left->find_best_fit(size, current_best_size, best_fit);
        }
        if (right != NULL)
        {
            best_fit = right->find_best_fit(size, current_best_size, best_fit);
        }
        return best_fit;
    }

    int alloc(int size, char pid)
    {
        int best_fit_value = 0;
        BuddyMemoryManager *best_fit = find_best_fit(size, best_fit_value, NULL);
        if (!best_fit)
        {
            cout << "Not enough memory" << endl;
            return 0;
        }
        else
        {
            while ((best_fit->node_size)/2 >= size && (best_fit->node_size)/2 > min_split)
            {
                best_fit->left = new BuddyMemoryManager("Free Block", best_fit->node_size / 2);
                best_fit->right = new BuddyMemoryManager("Free Block", best_fit->node_size / 2);
                best_fit->node_name = "Internal Node";
                best_fit = best_fit->left;
            }
            best_fit->node_name = pid;
            best_fit->allotted_size = size;
            best_fit->last_freed = 0;
            process_to_node[pid] = best_fit;
        }
        return 1;
    }

    int dealloc(char name)
    {
        if(process_to_node.find(name) != process_to_node.end())
        {
            BuddyMemoryManager *node = process_to_node[name];
            node->node_name = "Free Block";
            node->allotted_size = 0;
            node->last_freed = timestamp++;
            process_to_node.erase(name);
        }
        else
        {
            cout << "Process not found" << endl;
            return 0;
        }
        merge();
        return 1;
    }

    void merge()
    {
        if (left && right)
        {
            left->merge();
            right->merge();
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