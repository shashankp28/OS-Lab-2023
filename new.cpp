#include <atomic>
#include <vector>

template <typename T>
struct atomwrapper
{
    std::atomic<T> _a;

    atomwrapper()
        : _a()
    {
    }

    atomwrapper(const std::atomic<T> &a)
        : _a(a.load())
    {
    }

    atomwrapper(const atomwrapper &other)
        : _a(other._a.load())
    {
    }

    atomwrapper &operator=(const atomwrapper &other)
    {
        _a.store(other._a.load());
    }
};

int main(void)
{
    std::vector<atomwrapper<int>> v_a;
    std::atomic<int> a_i(1);
    v_a.push_back(a_i);
    return 0;
}