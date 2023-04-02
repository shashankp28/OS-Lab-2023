#include <iostream>
#include <fstream>
#include <string>

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


int main(){

    std::ifstream file("input1.txt");
    std::string line;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            std::cout << line << std::endl;
        }
        file.close();
    }
    else
    {
        std::cerr << "Unable to open file" << std::endl;
    }

    return 0;
}