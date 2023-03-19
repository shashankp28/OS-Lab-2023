for (auto i : requests)
{

    if (memory_frames.find(i) != memory_frames.end())
        continue;

    pageFaults++;
    if (memory_frames.size() >= memory_size)
    {
        int index = rand() % memory_size;
        auto itr = memory_frames.begin();
        advance(itr, index);
        memory_frames.erase(itr);
        swap_space.insert(*itr);
        if (swap_space.size() > swap_size)
        {
            cout << "Error: Swap space is full\n";
            return 1;
        }
    }
    if (swap_space.find(i) != swap_space.end())
        swap_space.erase(i);
    memory_frames.insert(i);
}