/*
 * Approximation of pi using the rectangle method
 * Sequential version
 *
 * Pablo
 * 2026-06-03
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

double compute_pi(long n, int threads);

// Definition of a pointer to a function
typedef double (*integral_function_t)(long, int);
void timeFunction(integral_function_t fun, long n, int threads);
double timespecToSeconds(struct timespec ts_begin, struct timespec ts_end);

int main(int argc, char * argv[])
{
    if (argc == 1 || atoi(argv[1]) <= 0)
    {
        printf("Number not provided or is not a positive integer. Usage: %s <positive integer>\n", argv[0]);
        return 1;
    }

    long n = atol(argv[1]);
    timeFunction(compute_pi, n, 1);
    return 0;
}

double compute_pi(long n, int threads)
{
    double sum = 0.0;
    double width = 1.0 / n;
    for (long i = 0; i < n; i++)
    {
        double mid = (i + 0.5) * width;
        double height = 4.0 / (1.0 + mid * mid);
        sum += height;
    }
    return width * sum;
}

void timeFunction(integral_function_t fun, long n, int threads)
{
    // Variables to measure time (this one measures cpu time)
    // https://www.geeksforgeeks.org/cpp/how-to-measure-time-taken-by-a-program-in-c/
    /*
    clock_t start;
    clock_t end;
    double cpu_time_used;

    start = clock();
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    */


    // https://levelup.gitconnected.com/8-ways-to-measure-execution-time-in-c-c-48634458d0f9
    struct timespec ts_wall_begin, ts_wall_end;
    struct timespec ts_cpu_begin, ts_cpu_end;
    double elapsed_wall;
    double elapsed_cpu;

    clock_gettime(CLOCK_REALTIME, &ts_wall_begin);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts_cpu_begin);

    // Call the function
    double result = fun(n, threads);

    clock_gettime(CLOCK_REALTIME, &ts_wall_end);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts_cpu_end);

    // Compute the times elapsed
    elapsed_wall = timespecToSeconds(ts_wall_begin, ts_wall_end);
    elapsed_cpu = timespecToSeconds(ts_cpu_begin, ts_cpu_end);

    printf("N: %ld | Result: %.15lf | Time: %lf | CPU: %lf\n", n, result, elapsed_wall, elapsed_cpu);
}

// Compute the total amount of seconds from a timespec
double timespecToSeconds(struct timespec ts_begin, struct timespec ts_end)
{
    long seconds;
    long nanoseconds;

    seconds = ts_end.tv_sec - ts_begin.tv_sec;
    nanoseconds = ts_end.tv_nsec - ts_begin.tv_nsec;
    return seconds + nanoseconds * 1e-9;
}
