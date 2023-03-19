for (int i = 0; i < requests.size(); i++)
{
    if (memory_frames.find(requests[i]) != memory_frames.end())
    {
        memory_frames[requests[i]] = i;
        continue;
    }

    pageFaults++;
    if (memory_frames.size() >= memory_size)
    {
        int swap = get_lru_request(memory_frames);
        cout << swap << endl;
        memory_frames.erase(swap);
        swap_space.insert(swap);
        if (swap_space.size() > swap_size)
        {
            cout << "Error: Swap space is full\n";
            return 1;
        }
    }
    if (swap_space.find(requests[i]) != swap_space.end())
        swap_space.erase(requests[i]);
    memory_frames[requests[i]] = i;
}