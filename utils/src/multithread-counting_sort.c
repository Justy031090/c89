#include <pthread.h> /*thread operations*/
#include <stddef.h> /*size_t*/
#include <stdlib.h> /*malloc*/
#include <ctype.h> /*tolower*/
#include <string.h> /*strlen*/
#include <stdio.h> /*printf*/
#include <time.h> /*clock*/
#include <assert.h>

#define NUM_OF_LETTER 26
#define NUM_OF_THREADS 2
#define MAX_STRINGS 150
#define MAX_STRING_LENGTH 30
#define PATH_TO_DICT "/usr/share/dict/words"

typedef struct 
{
    char **arr;
    size_t start;
    size_t end;
    size_t max_len;
} thread_data_t;

void *CountingSort(void *data);

char **MultiThreadSort(char **unsorted, size_t size, size_t num_of_threads)
{
    pthread_t threads[NUM_OF_THREADS];
    size_t i, j, start = 0, max = 0;
    size_t portion = size / NUM_OF_THREADS;
    size_t remainder = size % NUM_OF_THREADS;
    thread_data_t thread_data[NUM_OF_THREADS];

    (void)num_of_threads;

    /* Convert all strings to lowercase */
    for(i = 0; i < size; ++i)
    {
        for(j = 0; j < strlen(unsorted[i]); ++j)
        {
            unsorted[i][j] = tolower(unsorted[i][j]);
        }
    }

    for(i = 0; i < NUM_OF_THREADS; ++i)
    {
        thread_data[i].arr = unsorted;
        thread_data[i].start = start;
        thread_data[i].end = start + portion;
        thread_data[i].max_len = max;

        if(remainder > i)
        {
            thread_data[i].end++;
        }

        start = thread_data[i].end;

        pthread_create(&threads[i], NULL, CountingSort, &thread_data[i]);
    }

    for(i = 0; i < NUM_OF_THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    return unsorted;
}

void *CountingSort(void *data)
{
    int count[NUM_OF_LETTER] = {0};
    char **words = NULL;
    char **output = NULL;
    char c;
    size_t position, i, start, end, size, max;
    thread_data_t *thread_data = NULL;

    assert(data);
    thread_data = (thread_data_t *)data;
    words = thread_data->arr;
    start = thread_data->start;
    end = thread_data->end;
    size = end - start;
    max = thread_data->max_len;

    output = (char **)malloc(size * sizeof(char *));
    if(!output) return NULL;

    for(position = max - 1; position < max; --position)
    {
        for(i = start; i < end; ++i)
        {
            c = strlen(words[i]) > position ? words[i][position] : 0;
            count[c - 'a' + 1]++;
        }
        for(i = 1; i <= NUM_OF_LETTER; ++i)
        {
            count[i] += count[i - 1];
        }

        for(i = end - 1; i >= start; --i)
        {
            c = strlen(words[i]) > position ? words[i][position] : 0;
            output[--count[c - 'a' + 1]] = words[i];
        }

        for(i = start; i < end; ++i)
        {
            words[i] = output[i - start];
        }
    }

    free(output);
    pthread_exit(NULL);
}

void PrintArray(char **arr, size_t size)
{
    size_t i;
    for(i = 0; i < size; ++i)
    {
        printf("%s\n", arr[i]);
    }
}

int main()
{
    FILE *dictionary = fopen(PATH_TO_DICT, "r");
    char *arr[MAX_STRINGS];
    char buffer[MAX_STRING_LENGTH];
    size_t i = 0, j = 0;
    clock_t start, stop;

    if(dictionary == NULL)
    {
        printf("failed to open dictionary\n");
        return 0;
    }


    while(i < MAX_STRINGS && fgets(buffer, sizeof(buffer), dictionary) != NULL)
    {
        
        buffer[strcspn(buffer, "\n")] = 0;

        arr[i] = (char *)malloc((strlen(buffer) + 1) * sizeof(char));
        strcpy(arr[i], buffer);
        i++;
    }

    fclose(dictionary);


    printf("Original Array:\n");
    PrintArray(arr, i);

    start = clock();
    MultiThreadSort(arr, i, NUM_OF_THREADS);
    stop = clock();

    printf("\nSorted Array:\n");
    PrintArray(arr, i);

    printf("\nTime taken to sort: %ld seconds\n", (stop - start)/CLOCKS_PER_SEC);
    for (j = 0; j < i; ++j)
    {
        free(arr[j]);
    }


    return 0;
}
