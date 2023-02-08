rmp = &schedproc[proc_nr_n];
if (rmp->priority < MIN_USER_Q)
{
    rmp->priority -= 1; /* increase priority */
}
