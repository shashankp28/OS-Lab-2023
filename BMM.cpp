#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <sstream>
#include <bits/stdc++.h>

using namespace std;

class BuddyMemoryManager
{

private:
    string node_name;
    int node_size;
    int last_freed;
    BuddyMemoryManager *left;
    BuddyMemoryManager *right;
    int allotted_size;
    static int time;
    unordered_map<char, BuddyMemoryManager *> process_to_node;

public:
    BuddyMemoryManager(string node_n, int node_s)
    {
        node_name = node_n;
        node_size = node_s;
        allotted_size = 0;
        last_freed = time++;
        left = NULL;
        right = NULL;
    }

    void find_best_fits(int size, int current_best_size, BuddyMemoryManager *best_fit)
    {
        if (node_name == "Free Block" && !this->left && !this->right)
        {
            if (!best_fit)
            {
                best_fit = this;
                current_best_size = node_size;
            }
            else if (this->node_size < current_best_size)
            {
                best_fit = this;
                current_best_size = node_size;
            }
            else if (this->node_size == current_best_size)
            {
                if (this->last_freed > best_fit->last_freed)
                {
                    best_fit = this;
                    current_best_size = node_size;
                }
            }
            return;
        }
        if (left != NULL)
        {
            left->find_best_fits(size, current_best_size, best_fit);
        }
        if (right != NULL)
        {
            right->find_best_fits(size, current_best_size, best_fit);
        }
        return;
    }

    int alloc(int size, char pid)
    {
        BuddyMemoryManager *best_fit = NULL;
        int current_best_size = 0;
        this->find_best_fits(size, current_best_size, best_fit);
        if (best_fit == NULL)
        {
            cout << "Not enough memory" << endl;
            return 1;
        }
        else
        {
            while ((best_fit->node_size) / 2 < size)
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
        return 0;
    }

    void dealloc(char name)
    {
        BuddyMemoryManager *node = process_to_node[name];
        node->node_name = "Free Block";
        node->allotted_size = 0;
        node->last_freed = time++;
        process_to_node.erase(name);
        this->merge();
        return;
    }

    void merge()
    {
        if (this->left && this->right)
        {
            this->left->merge();
            this->right->merge();
        }
        else if (this->left->node_name == "Free Block" && this->right->node_name == "Free Block")
        {
            this->left = NULL;
            this->right = NULL;
            this->node_name = "Free Block";
        }
        return;
    }

    void print()
    {
        if (this->left && this->right)
        {
            this->left->print();
            this->right->print();
        }
        else
        {
            if (node_name == "Free Block")
            {
                cout << this->node_name << ": " << this->node_size << endl;
            }
            else
            {
                cout << this->node_name << ": " << this->allotted_size << endl;
            }
        }
        return;
    }
};

int main()
{

    ifstream file("input1.txt");
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

                if (file.fail() || file.eof())
                    break;

                string id;
                int size;

                std::stringstream ss(line);
                ss >> id >> size;

                cout << t << " " << U << " " << L << " " << id << " " << size << endl;
                getline(file, line);
            }
        }
        file.close();
    }
    else
    {
        cerr << "Unable to open file" << endl;
    }

    return 0;
}