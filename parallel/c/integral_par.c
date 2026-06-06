/*
 * Approximation of pi using the rectangle method
 * Parallel version using threads
 *
 * Pablo
 * 2026-06-03
 * 
 * Speedup measurements :
 * T1 (sequential, from integral_seq): 5.895066 s
 * Tp (p threads): 60 threads          0.532374 s
 * Sp = T1 / Tp:                       11.07316660843693
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>



typedef struct {
    int id;
    long start;
    long end;
    double width;
    double * result;
    pthread_mutex_t * mutex;
} data_t;

double compute_pi(long n, int threads);
void * sumRectangles(void * data);


typedef double (*integral_function_t)(long, int);
void timeFunction(integral_function_t fun, long n, int threads);
double timespecToSeconds(struct timespec ts_begin, struct timespec ts_end);

int main(int argc, char * argv[])
{
    if (argc == 1 || atoi(argv[1]) <= 0)
    {
        printf("Number not provided or is not a positive integer. Usage: %s <positive integer> [threads]\n", argv[0]);
        return 1;
    }

    long n = atol(argv[1]);
    int threads = 1;
    if (argc == 3)
    {
        threads = atoi(argv[2]);
    }

    timeFunction(compute_pi, n, threads);
    return 0;
}

// Approximate pi with the rectangle method splitting the rectangles among threads
double compute_pi(long n, int threads)
{
    double width = 1.0 / n;

    long rangeSize = n / threads;
    long remainder = n % threads;

    // Create an array of structures
    data_t thread_data[threads];
    pthread_t tids[threads];

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    int status = 0;

    double sum = 0.0;

    // Loop to create as many threads as requested
    for (int i = 0; i < threads; i++)
    {
        thread_data[i].id = i;
        thread_data[i].start =  i * rangeSize;
        thread_data[i].end =  (i + 1) * rangeSize;
        thread_data[i].width = width;
        thread_data[i].result = &sum;
        thread_data[i].mutex = &mutex;

        // Assign the remainder to the last thread
        if (i == threads - 1)
        {
            thread_data[i].end += remainder;
        }

        // Call the partial functions
        status = pthread_create(&tids[i], NULL, sumRectangles, &thread_data[i]);
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

    return width * sum;
}

// Sum the heights of the rectangles within a range, then add them to the shared sum
void * sumRectangles(void * data)
{
    // Cast the pointer into the desired type
    data_t * localData = (data_t *) data;
    double localSum = 0.0;

    for (long i = localData->start; i < localData->end; i++)
    {
        double mid = (i + 0.5) * localData->width;
        double height = 4.0 / (1.0 + mid * mid);
        localSum += height;
    }

    pthread_mutex_lock(localData->mutex);
    // Update the shared variable
    (*localData->result) += localSum;
    pthread_mutex_unlock(localData->mutex);

    pthread_exit(NULL);
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
