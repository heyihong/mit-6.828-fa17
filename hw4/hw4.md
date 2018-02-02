# Homework 4

## Part One

The implementation simply removes growproc() and modifies sz if n is non-negative. Otherwise, returns -1.

## Part Two

Modify trap() to handle T_PGFLT. If the fault address falls into user space, then try to allocate a physical page and map it. Otherwise, treat this page fault by default.
