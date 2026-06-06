#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

bool isPrime(int x);
unsigned long long sumPrimes(int n, int threads);

// Definition of a pointer to a function
typedef unsigned long long (*prime_function_t)(int, int);
void timeFunction(prime_function_t fun, int n, int threads);
double timespecToSeconds(struct timespec ts_begin, struct timespec ts_end);

int main(int argc, char * argv[])
{
    if (argc == 1 || atoi(argv[1]) <= 0)
    {
        printf("Number not provided or is not a positive integer. Usage: %s <positive integer>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    timeFunction(sumPrimes, n, 1);
    return 0;
}

unsigned long long sumPrimes(int n, int threads)
{
    unsigned long long total = 0;
    for (int i = n; i > 0; i--)
    {
        if (isPrime(i))
            total += i;
    }
    return total;
}

bool isPrime(int x)
{
    if (x < 2)
        return false;
    if (x == 2)
        return true;
    else {
        for (int i = 2; i <= ceil(sqrt(x)); i++)
            if (x % i == 0)
                return false;
        return true;
    }
    
}

void timeFunction(prime_function_t fun, int n, int threads)
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
    unsigned long long result = fun(n, threads);

    clock_gettime(CLOCK_REALTIME, &ts_wall_end);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts_cpu_end);

    // Compute the times elapsed
    elapsed_wall = timespecToSeconds(ts_wall_begin, ts_wall_end);
    elapsed_cpu = timespecToSeconds(ts_cpu_begin, ts_cpu_end);

    printf("N: %d | Result: %llu | Time: %lf | CPU: %lf\n", n, result, elapsed_wall, elapsed_cpu);
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