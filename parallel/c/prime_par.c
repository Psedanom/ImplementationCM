/*
 * Sum of all prime numbers smaller than or equal to n
 * Parallel version using threads
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

// Speedup measurements (fill in with your own runs):
// T1 (sequential, from prime_seq): 12.530334 s
// Tp (p threads): 75 threads       1.249991 s
// Sp = T1 / Tp:                    10.0243393752435 s

// Definition of the structure to share data with the threads
typedef struct {
    int id;
    int start;
    int end;
    // Pointer to the variable where the result will be accumulated
    unsigned long long * result;
    pthread_mutex_t * mutex;
} data_t;

bool isPrime(int x);
unsigned long long sumPrimes(int n, int threads);
void * sumRange(void * data);

// Definition of a pointer to a function
typedef unsigned long long (*prime_function_t)(int, int);
void timeFunction(prime_function_t fun, int n, int threads);
double timespecToSeconds(struct timespec ts_begin, struct timespec ts_end);

int main(int argc, char * argv[])
{
    if (argc == 1 || atoi(argv[1]) <= 0)
    {
        printf("Number not provided or is not a positive integer. Usage: %s <positive integer> [threads]\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int threads = 1;
    if (argc == 3)
    {
        threads = atoi(argv[2]);
    }

    timeFunction(sumPrimes, n, threads);
    return 0;
}

// Sum the prime numbers from 1 to n, splitting the range among threads
unsigned long long sumPrimes(int n, int threads)
{
    int rangeSize = n / threads;
    int remainder = n % threads;

    // Create an array of structures
    data_t thread_data[threads];
    pthread_t tids[threads];

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    int status = 0;

    // Variable to store the sum computed in parallel
    unsigned long long total = 0;

    // Loop to create as many threads as requested
    for (int i = 0; i < threads; i++)
    {
        thread_data[i].id = i;
        thread_data[i].start = i * rangeSize + 1;
        thread_data[i].end = (i + 1) * rangeSize;
        thread_data[i].result = &total;
        thread_data[i].mutex = &mutex;

        // Assign the remainder to the last thread
        if (i == threads - 1)
        {
            thread_data[i].end += remainder;
        }

        // Call the partial functions
        status = pthread_create(&tids[i], NULL, sumRange, &thread_data[i]);
        if (status == -1)
        {
            perror("ERROR: pthread_create");
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < threads; i++)
    {
        status = pthread_join(tids[i], NULL);
        if (status == -1)
        {
            perror("ERROR: pthread_join");
        }
    }

    return total;
}

// Sum the prime numbers within a range, then add them to the shared total
void * sumRange(void * data)
{
    // Cast the pointer into the desired type
    data_t * localData = (data_t *) data;
    unsigned long long localSum = 0;

    for (int i = localData->start; i <= localData->end; i++)
    {
        if (isPrime(i))
            localSum += i;
    }

    pthread_mutex_lock(localData->mutex);
    // Update the shared variable
    (*localData->result) += localSum;
    pthread_mutex_unlock(localData->mutex);

    pthread_exit(NULL);
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
