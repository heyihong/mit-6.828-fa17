# Homework 7
1. Explain in one sentence what happens.

Answer: the code will lead to panic("acquire")

2. Explain in a few sentences why the kernel panicked. You may find it useful to look up the stack trace (the sequence of %eip values printed by panic) in the kernel.asm listing.

Answer: From my experiments, two kinds of panic were observed and they are caused by ide interrupt and timer interrupt respectively.
- ide interrupt: When the kernel is executing iderw() and holding idelock, the caller receives an ide interrupt. It starts to execute ideintr() and acqiore idelock again that leads to panic("acquire"). For example,  if iderw finishes idestart(b) and disk device finishes its io operations immediately, the senario above will happen.
- timer interrupt: When the kernel is executing iderw and holding idelock, the caller receives an timer interrupt and yields its control of cpu. However, the yield function requires the depth of the thread's interrupt stack to be 1 while the actual depth is 2. As a result, panic("sched locks") occurs.   

3. Explain in a few sentences why the kernel didn't panic. Why do file_table_lock and ide_lock have different behavior in this respect?

Answer: There is no interrupt handler needs to acquire file_table_lock so in most time the kernel didn't panic, compared with ide_lock. However, with small probability, it may receive timer interrupt that leads to panic.

4. Why does release() clear lk->pcs[0] and lk->cpu before clearing lk->locked? Why not wait until after?

Answer: Because it aims to prevent race condition. If the code wait until after, it is probable that debug information is missed. For example, there are two cpu and the events are as followed:
```
cpu 0: clear lk->locked
cpu 1: set lk->locked
cpu 1: set lk->pcs[0] and lk->cpu
cpu 0: clear lk->pcs[0] and lk->cpu
```
