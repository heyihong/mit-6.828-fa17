# Homework 6
1. Why are there missing keys with 2 or more threads, but not with 1 thread?

Answer: There are race conditions in put operations with 2 or more threads. For example, there are two threads and its events are as follow:
```
0: e->next = n
1: e->next = n
0: *p = e
1: *p = e
```
As a result, some keys are missed.

2. Is it correct (i.e. have you eliminated missing keys?)? Is the two-threaded version faster than the single-threaded version?

Answer: Yes, after using mutex, it is correct. However, its two-threaded put performance is worse than its single-threaded version. My method is to assign each bucket a mutex for put operations and not change get operation. As a result, its get performance scales well with number of cores but its put performance is worse than the single-thread one. The reason are: 1. NBUCKET is too small so that it is very likely that threads are acquiring lock in the same bucket; 2. pthread library is 1:1 model, so its context-switch overheads are non-negligent for put operations. 

