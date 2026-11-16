
#include <stdio.h>
#include <stddef.h>

typedef struct {
    const char *name;
    int arrival;
    int burst;
} Job;

static void simulate(const char *title, const Job jobs[], size_t n) {
    int clock = 0;
    int total_wait = 0;
    printf("\n%s\n", title);
    printf("Job Arrival Burst Start Finish Wait Turnaround\n");
    for (size_t i = 0; i < n; ++i) {
        /* If no job is ready, the CPU is idle until this arrival. */
        if (clock < jobs[i].arrival) {
            clock = jobs[i].arrival;
        }
        int start = clock;
        int wait = start - jobs[i].arrival;
        int finish = start + jobs[i].burst;
        int turnaround = finish - jobs[i].arrival;
        total_wait += wait;
        printf("%s %7d %5d %5d %6d %4d %10d\n", jobs[i].name,
               jobs[i].arrival, jobs[i].burst, start, finish,
               wait, turnaround);
        clock = finish;
    }
    printf("Average waiting time: %.2f\n", (double)total_wait / n);
}

int main(void) {
    const Job long_first[] = {{"A", 0, 5}, {"B", 0, 2}, {"C", 0, 1}};
    const Job short_first[] = {{"C", 0, 1}, {"B", 0, 2}, {"A", 0, 5}};
    const Job staggered[] = {{"A", 0, 5}, {"B", 1, 2}, {"C", 2, 1}};
    const Job idle[] = {{"A", 2, 2}, {"B", 6, 1}};
    simulate("1. Long-first tie order", long_first, 3);
    simulate("2. Short-first tie order", short_first, 3);
    simulate("3. Staggered arrivals", staggered, 3);
    simulate("4. Idle CPU intervals", idle, 2);
    return 0;
}
