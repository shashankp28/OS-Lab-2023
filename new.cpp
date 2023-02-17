#include <vector>
#include <atomic>

int main()
{
    std::vector<std::atomic<bool>> global_vec;

    // Push two atomic bools into the vector
    global_vec.push_back(std::atomic<bool>{true});
    global_vec.push_back(std::atomic<bool>{false});

    // Print the values of the atomic bools
    std::cout << "global_vec[0]: " << global_vec[0] << std::endl;
    std::cout << "global_vec[1]: " << global_vec[1] << std::endl;

    return 0;
}
