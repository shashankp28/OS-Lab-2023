#include <iostream>

using namespace std;

class BuddyMemoryManager
{

private:
    string node_name;
    int node_size;
    BuddyMemoryManager *left;
    BuddyMemoryManager *right;

public:
    BuddyMemoryManager(string node_n, int node_s)
    {
        node_name = node_n;
        node_size = node_s;
        left = NULL;
        right = NULL;
    }

    void alloc(int size)
    {
        return;
    }

    void dealloc(string name)
    {
        return;
    }

    void print()
    {
        return;
    }
};