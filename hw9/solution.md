# Homework 9
In my implementation, I use barrier_mutex to protect critical section and barrier_cond to wait for bstate.round to increase. Every time a thread enters barrier function, it increases bstate.nthread by 1. Then, if bstate.nthread is equal to nthread, it increases bstate.round, clears bstate.nthread to 0 and broadcast the event.  
