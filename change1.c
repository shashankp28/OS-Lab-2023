static void balance_queues(minix_timer_t *tp)
{
    // struct schedproc *rmp;
    // int proc_nr;

    // for (proc_nr = 0, rmp = schedproc; proc_nr < NR_PROCS; proc_nr++, rmp++)
    // {
    // 	if (rmp->flags & IN_USE)
    // 	{
    // 		if (rmp->priority > rmp->max_priority)
    // 		{
    // 			rmp->priority -= 1; /* increase priority */
    // 			schedule_process_local(rmp);
    // 		}
    // 	}
    // }

    // set_timer(&sched_timer, balance_timeout, balance_queues, 0);
}
