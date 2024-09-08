#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <omp.h>

#define NUM_THREADS 4
#define NUMBER 1000000000

typedef struct
{
    int start;
    int end;
    int number;
    long long int *result;
} thread_args_t;

void *sum_divisors(void *args)
{
    thread_args_t *threadArgs = (thread_args_t *)args;
    long long int sum = 0;
    int i = 0;
    for (i = threadArgs->start; i <= threadArgs->end; i++)
    {
        if (threadArgs->number % i == 0)
        {
            sum += i;
        }
    }
    *(threadArgs->result) = sum;
    pthread_exit(NULL);
}

long long int multithreaded_sum_divisors(int number)
{
    pthread_t threads[NUM_THREADS];
    thread_args_t threadArgs[NUM_THREADS];
    int range = number / NUM_THREADS;
    long long int results[NUM_THREADS] = {0};
    int i = 0;

    for (i = 0; i < NUM_THREADS; i++)
    {
        threadArgs[i].start = i * range + 1;
        threadArgs[i].end = (i == NUM_THREADS - 1) ? number : (i + 1) * range;
        threadArgs[i].number = number;
        threadArgs[i].result = &results[i];
        pthread_create(&threads[i], NULL, sum_divisors, (void *)&threadArgs[i]);
    }

    long long int total_sum = 0;
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
        total_sum += results[i];
    }

    return total_sum;
}

long long int loop_sum_divisors(int number)
{
    long long int sum = 0;
    int i;
#pragma omp parallel for
    for (i = 1; i <= number; i++)
    {
        if (number % i == 0)
        {
            sum += i;
        }
    }
    return sum;
}

int main()
{
    int number = NUMBER;
    clock_t start_time, end_time;
    double multithreaded_time, loop_time;
    long long int multithreaded_sum, loop_sum;

    start_time = clock();
    multithreaded_sum = multithreaded_sum_divisors(number);
    end_time = clock();
    multithreaded_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Sum of divisors (multithreaded) for %d: %lld\n", number, multithreaded_sum);
    printf("Time taken (multithreaded): %.2f seconds\n", multithreaded_time);

    start_time = clock();
    loop_sum = loop_sum_divisors(number);
    end_time = clock();
    loop_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Sum of divisors (loop) for %d: %lld\n", number, loop_sum);
    printf("Time taken (loop): %.2f seconds\n", loop_time);

    return 0;
}
