#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <sstream>

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

    void merge(){
        return;
    }

    void print()
    {
        return;
    }
};


int main(){

    ifstream file("input1.txt");
    string line;
    int test_cases;

    if (file.is_open())
    {
        getline(file, line);
        test_cases = stoi(line);
        getline(file, line);

        for (int t = 0; t< test_cases; t++){
            getline(file, line);
            int U, L;
            std::stringstream ss(line);
            ss >> U >> L;
            getline(file, line);

            U = pow(2, U);
            L = pow(2, L);

            BuddyMemoryManager *root = new BuddyMemoryManager("Free Block", U);

            getline(file, line);
            while (line != ""){
                
                if (file.fail() || file.eof()) break;

                string id;
                int size;

                std::stringstream ss(line);
                ss >> id >> size;

                cout << t <<" "<< U <<" "<< L<<" "<< id <<" "<< size << endl;
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