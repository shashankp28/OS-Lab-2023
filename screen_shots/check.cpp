//  Tranformation 1
{
    mtx.lock();
    trans1[0] = i;
    trans1[1] = j;
    mtx.unlock();
}

//  Tranformation 2
{
    mtx.lock();
    trans2[0] = i;
    trans2[1] = j;
    if (trans1[0] < trans2[0] || (trans1[0] == trans2[0] && trans1[1] < trans2[1]))
    {
        cout << "T2 overtook T1 " << trans2[0] << " " << trans2[1] << "\n";
    }
    mtx.unlock();
}