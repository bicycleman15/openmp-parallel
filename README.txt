Parallel Assignment 2
------------------------

Jatin Prakash, 2018CS10344
Siddharth Grover, 2018CS10392

Explaination of Approaches
---------------------------

Strategy 0: This is the same code as provided to us with no parallelism

Strategy 1: Here we just add omp parallel for pragma on top of the nested for loops. We can't parallelize the outermost loop, due to the
            dependency from j -> j+1. Each iteration of the loops is parallel. However, only when one loop finishes execution, only then the nested loop below it
            can be run.

Strategy 2: Here we add 2 parallel sections inside the outermost loop so that the 2 nested loop can be run in parallel.

Strategy 3: This combines strategy 1 and 2 in one strategy. While the 2 nested loops run in parallel, each iteration of the nested loop is also parallel.

Strategy 4: Open MPI
@Siddharth


Removing Data Races
-------------------

In the initial code, there is a data race for L[j][j] if it is run in parallel, However since it is not run in parallel, it not a concern.
But to run it in parallel, we need to make some changes so that loop iterations are independent. Do this, we used a simple fix. We explicetly calculate the 
value of L[j][j] first, and then run the 2 loops in parallel, in this way the data race is avoided.