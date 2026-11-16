# First-Come, First-Served CPU Scheduling

**Etornam Agbemabiese — COMP 211 Checkoff 3**

## Introduction

First-Come, First-Served (FCFS) is a CPU scheduling algorithm that selects processes in the order they enter the ready queue. It works like a checkout line: whoever joins the line first gets served first.

FCFS is non-preemptive. A newly arriving process does not interrupt the process currently using the CPU. The running process can still finish or block while waiting for something such as input or output.

This project explores a question: **How does the order of jobs affect how long they wait for the CPU?**

## What this project explores

The C program in `src/fcfs.c` simulates four scenarios:

1. A long job followed by two shorter jobs.
2. The same jobs in a different initial queue order.
3. Jobs that arrive at different times.
4. Gaps between arrivals when the CPU is idle.

Each job has a name, an arrival time, and a CPU burst length. A CPU burst is a continuous stretch of CPU work. The simulation prints each job's start time, finish time, waiting time, and turnaround time, followed by the average waiting time.

The program connects to COMP 211 through C structs, arrays, functions, and a loop that updates simulated state.

## Assumptions

- There is one CPU.
- Every job has one CPU burst and no input/output activity.
- Switching between jobs takes no time.
- Jobs are already listed in nondecreasing arrival order. Array order breaks ties.
- All time values are abstract units, not measured performance on a real computer.
- The program uses fixed, small inputs with nonnegative arrival times and positive burst lengths. It is a teaching simulation, not an operating-system scheduler or a general input-processing tool.

FCFS does not need to know burst lengths to choose which job goes next. This simulation uses the lengths to calculate completion times.

## How to compile and run

You need a C compiler such as GCC or Clang. Open a terminal in the folder containing this README and run:

```sh
cc -std=c11 -Wall -Wextra -Werror -pedantic src/fcfs.c -o fcfs
./fcfs
```

You can replace `cc` with `gcc` or `clang`. The program requires no input and prints all four experiments. On Windows with GCC available, compile with `-o fcfs.exe` and run `.\fcfs.exe` in PowerShell.

To compare your output with the included reference output:

```sh
./fcfs > actual-output.txt
diff -u expected-output.txt actual-output.txt
```

If `diff` prints nothing, the outputs match.

## Calculations

For this one-burst model:

| Quantity | Calculation |
| --- | --- |
| Start time | Maximum of the previous job's finish and this job's arrival |
| Waiting time | Start minus arrival |
| Finish time | Start plus CPU burst |
| Turnaround time | Finish minus arrival |
| Average waiting time | Total waiting time divided by number of jobs |

For a process with several CPU bursts, total waiting includes every interval spent in the ready queue. Start minus initial arrival alone would not measure all of that waiting.

## Experiment 1: Long job first

All jobs arrive at time 0. Their tie order is A, B, C.

| Job | Arrival | CPU burst | Start | Finish | Wait | Turnaround |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| A | 0 | 5 | 0 | 5 | 0 | 5 |
| B | 0 | 2 | 5 | 7 | 5 | 7 |
| C | 0 | 1 | 7 | 8 | 7 | 8 |

**Average waiting time: (0 + 5 + 7) / 3 = 4.00 units.**

C only needs one unit of CPU time, but it waits seven units before starting.

## Experiment 2: Different initial queue

The same jobs still arrive at time 0, but their tie order is C, B, A.

| Job | Arrival | CPU burst | Start | Finish | Wait | Turnaround |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| C | 0 | 1 | 0 | 1 | 0 | 1 |
| B | 0 | 2 | 1 | 3 | 1 | 3 |
| A | 0 | 5 | 3 | 8 | 3 | 8 |

**Average waiting time: (0 + 1 + 3) / 3 = 1.33 units, rounded.**

This changes the initial tie order. FCFS itself does not sort the jobs by burst length. Deliberately selecting the shortest available job would be a different scheduling policy: Shortest Job First.

## Experiment 3: Different arrival times

A arrives at 0, B at 1, and C at 2, with bursts of 5, 2, and 1 respectively. Their start times remain 0, 5, and 7. Their waits are now **0, 4, and 5**, giving an average of **3.00 units**.

This shows why waiting time must subtract arrival time. B does not begin waiting until time 1.

## Experiment 4: An idle CPU

A arrives at 2 and runs for 2 units. B arrives at 6 and runs for 1 unit. The CPU is idle from 0 to 2 and again from 4 to 6. Both jobs start as soon as they arrive, so the average wait is **0.00 units**.

The condition below advances the clock when the next job has not arrived:

```c
if (clock < jobs[i].arrival) {
    clock = jobs[i].arrival;
}
```

## Most interesting findings

- **Queue order changes average waiting time.** The first two experiments use the same jobs and both require eight units of CPU work, but average waiting changes from 4.00 to 1.33 units.
- **A short job can spend much longer waiting than running.** In the first experiment, C waits seven units for one unit of work. This illustrates the convoy effect: shorter jobs wait behind a long job.
- **Waiting time and turnaround time measure different things.** C's wait is seven units, while its turnaround is eight because turnaround includes its CPU work.
- **Idle time is different from waiting time.** The CPU can sit idle before a job arrives without that job waiting in the ready queue.

## Try another experiment

In the `long_first` array, change A's burst from 5 to 20. Predict the output before compiling and running again.

The expected waits are 0, 20, and 22, giving an average of **14.00 units**. The CPU performs 23 units of work. To compare the same workload in another order, also change A's burst to 20 in `short_first`.

## Files

- `README.md`: topic explanation, experiments, results, and run instructions.
- `src/fcfs.c`: C code for all four experiments.
- `expected-output.txt`: output from compiling and running the supplied program.

## Reference and AI assistance

Arpaci-Dusseau, R. H., & Arpaci-Dusseau, A. C. *Operating Systems: Three Easy Pieces*, Chapter 7, “Scheduling: Introduction.” Sections 7.2–7.4 explain scheduling metrics, FCFS/FIFO, the convoy effect, and Shortest Job First.

https://pages.cs.wisc.edu/~remzi/OSTEP/cpu-sched.pdf

ChatGPT assisted with generating the C example and creating this readme file. 