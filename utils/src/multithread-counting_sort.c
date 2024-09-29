
#define _POSIX_C_SOURCE 199309L /*clock_getttime*/
#include <pthread.h> /*thread operations*/
#include <stddef.h>  /*size_t*/
#include <stdlib.h>  /*malloc, qsort, srand */
#include <ctype.h>   /*tolower*/
#include <string.h>  /*strlen*/
#include <stdio.h>   /*printf*/
#include <time.h>    /*clock*/
#include <assert.h>

#define NUM_OF_LETTER 26
#define NUM_OF_THREADS 2
#define MAX_STRINGS 100000
#define MAX_STRING_LENGTH 30
#define PATH_TO_DICT "/usr/share/dict/words"

typedef struct
{
    char **arr;
    size_t start;
    size_t end;
    size_t max_len;
} thread_data_t;

char *dict_buffer = NULL;
char **word_pointer = NULL;
size_t word_count = 0;

int Randomize(const void *a, const void *b)
{
    return (rand() % 2) - 1;
}

int CompareStrings(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

int LoadDict()
{
    char *word = NULL;
    size_t file_size = 0, read_size = 0;
    FILE *dict = fopen(PATH_TO_DICT, "r");
    if (!dict)
        return -1;

    fseek(dict, 0, SEEK_END);
    file_size = ftell(dict);
    rewind(dict);

    dict_buffer = malloc(file_size + 1);
    if (!dict_buffer)
        return -1;

    read_size = fread(dict_buffer, 1, file_size, dict);
    if (read_size != file_size)
        return -1;

    dict_buffer[file_size] = '\0';
    fclose(dict);

    word_pointer = malloc(MAX_STRINGS * sizeof(char *));
    if (!word_pointer)
        return -1;

    word = strtok(dict_buffer, "\n");
    while (word && word_count < MAX_STRINGS)
    {
        word_pointer[word_count++] = word;
        word = strtok(NULL, "\n");
    }

    return 0;
}

void ShuffleWOrds()
{
    srand(time(NULL));
    qsort(word_pointer, word_count, sizeof(char *), Randomize);
}

void *ThreadSort(void *arg)
{
    thread_data_t *data = (thread_data_t *)arg;
    qsort(data->words + data->start, data->end - data->start, sizeof(char *), CompareStrings);
    return NULL;
}

int MultiThreadForSHuffle(int num_of_threads)
{
    pthread_t threads[NUM_OF_THREADS];
    thread_data_t thread_data[NUM_OF_THREADS];
    size_t portion = word_count / NUM_OF_THREADS;
    size_t remainder = word_count % NUM_OF_THREADS;
    size_t i = 0, min_idx = 0;
    char **temp = NULL;

    for (i = 0; i < NUM_OF_THREADS, ++i)
    {
        thread_data->arr = word_pointer;
        thread_data[i].start = i * portion + (i < remainder ? i : remainder);
        thread_data[i].end = (i + 1) * portion + (i + 1 < remainder ? i + 1 : remainder);
        pthread_create(&threads[i], NULL, ThreadSort, &thread_data[i]);
    }

    for (i = 0; i < NUM_OF_THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    temp = malloc(word_count * sizeof(char *));
    if (!temp)
        return -1;

    for (i = 0; i < word_count; ++i)
    {
        min_idx = 0;
        for (j = 1; j < NUM_OF_THREADS; ++j)
        {
            if (thread_data[j].start < thread_data[j].end)
            {
                if (thread_data[min_idx].start >= thread_data[min_idx].end ||
                    strcmp(word_pointer[thread_data[j].start], word_pointer[thread_data[min_idx].start]))
                {
                    min_idx = j;
                }
            }
        }
        temp[i] = word_pointer[thread_data[min_idx].start++];
    }
    memcpy(word_pointer, temp, word_count * sizeof(char *));
    free(temp);
}

char **MultiThreadSort(char **unsorted, size_t size, size_t num_of_threads)
{
    pthread_t threads[NUM_OF_THREADS];
    size_t i = 0, j = 0, start = 0, max = 0;
    size_t portion = size / NUM_OF_THREADS;
    size_t remainder = size % NUM_OF_THREADS;
    thread_data_t thread_data[NUM_OF_THREADS];

    (void)num_of_threads;

    /* Convert all strings to lowercase */
    for (i = 0; i < size; ++i)
    {
        for (j = 0; j < strlen(unsorted[i]); ++j)
        {
            unsorted[i][j] = tolower(unsorted[i][j]);
        }
    }

    for (i = 0; i < NUM_OF_THREADS; ++i)
    {
        thread_data[i].arr = unsorted;
        thread_data[i].start = start;
        thread_data[i].end = start + portion;
        thread_data[i].max_len = max;

        if (remainder > i)
        {
            thread_data[i].end++;
        }

        start = thread_data[i].end;

        pthread_create(&threads[i], NULL, CountingSort, &thread_data[i]);
    }

    for (i = 0; i < NUM_OF_THREADS; ++i)
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
    if (!output)
        return NULL;

    for (position = max - 1; position < max; --position)
    {
        for (i = start; i < end; ++i)
        {
            c = strlen(words[i]) > position ? words[i][position] : 0;
            count[c - 'a' + 1]++;
        }
        for (i = 1; i <= NUM_OF_LETTER; ++i)
        {
            count[i] += count[i - 1];
        }

        for (i = end - 1; i >= start; --i)
        {
            c = strlen(words[i]) > position ? words[i][position] : 0;
            output[--count[c - 'a' + 1]] = words[i];
        }

        for (i = start; i < end; ++i)
        {
            words[i] = output[i - start];
        }
    }

    free(output);
    pthread_exit(NULL);
}

void PrintArray(size_t count)
{
    size_t i = 0;
    for (i = 0; i < count && i < word_count; ++i)
    {
        printf("%s\n", word_pointer[i]);
    }
    printf("\n");
}

int main()
{
    struct timespec start, end;
    double time_taken;
    int num_threads, num_experiments;
    int thread_counts[] = {1, 2, 4, 8};
    size_t i = 0;
    char **word_pointers_copy = NULL;

    LoadDict();
    printf("Loaded %ld words from dictionary.\n", word_count);

    printf("First 10 words before shuffling:\n");
    print_words(10);

    ShuffleWOrds();
    printf("First 10 words after shuffling:\n");
    print_words(10);

    num_experiments = sizeof(thread_counts) / sizeof(thread_counts[0]);

    for (i = 0; i < num_experiments; i++)
    {
        num_threads = thread_counts[i];

        word_pointers_copy = malloc(word_count * sizeof(char *));
        if (word_pointers_copy == NULL)
            return -1;

        memcpy(word_pointers_copy, word_pointer, word_count * sizeof(char *));

        clock_gettime(CLOCK_MONOTONIC, &start);
        MultiThreadForSHuffle(num_threads);
        clock_gettime(CLOCK_MONOTONIC, &end);


        time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        printf("Time taken with %d threads: %f seconds\n", num_threads, time_taken);

        printf("First 10 words after sorting:\n");
        print_words(10);

        memcpy(word_pointer, word_pointers_copy, word_count * sizeof(char *));
        free(word_pointers_copy);
    }

    free(dict_buffer);
    free(word_pointer);

    return 0;
}