#include <pthread.h> /*thread operations*/
#include <stddef.h> /*size_t*/
#include <stdlib.h> /*Calloc*/
#include <assert.h>


    /*Each thread should recieve a portion of the unsored array*/
    /*Each thread should perform a count sort on this portion*/
    /*Main thread recieves the sorted mini arrays*/
typedef struct 
{
    int *arr;
    size_t start;
    size_t end;
} thread_data_t;


static int MaxArrayValue(int *numbers, size_t size);
void *CountingSort(void *data);

int *MultiThreadSort(int *unsorted, size_t size, size_t num_of_threads)
{
    pthread_t threads[num_of_threads];
    size_t i = 0, start = 0;
    size_t portion = size / num_of_threads;
    size_t remainder = size % num_of_threads;
    thread_data_t thread_data[num_of_threads];
    for(;i<num_of_threads;++i)
    {
        thread_data[i].arr = unsorted;
        thread_data[i].start = start;
        thread_data[i].end = start + portion;

        if(remainder > i)
        {
            thread_data[i].end++;
        }

        start = thread_data[i].end;

        pthread_create(&threads[i], NULL, CountingSort, &thread_data[i]);
    };

    for(i = 0; i< num_of_threads; ++i)
    {
        pthread_join(threads[i],NULL);
    }

    return unsorted;
}




void *CountingSort(void *data)
{
	int max = 0;
	int *count = NULL, *numbers = NULL;
	size_t i = 0, j = 0, start, end, size;
    thread_data_t *thread_data = NULL;

	assert(data);
    size = end - start;
    thread_data = (thread_data_t *)data;
    numbers = thread_data->arr;
    start = thread_data->start;
    end = thread_data->end;

	max = MaxArrayValue(numbers[start],size );
    count = calloc(max +1, sizeof(int));

	if(NULL == count)
    {
        pthread_exit(NULL);
    }

    for(i = start, i< end; ++i)
    {
        count[numbers[i]]++;
    }
	
	for(i = 0; (int)i<=max; ++i)
	{
		while(count[i]>0)

		{
			numbers[j++] = (int)i;
			count[i]--;
		}
	}
	free(count);
	pthread_exit(NULL);
}

static int MaxArrayValue(int *numbers, size_t size)
{
    int i = 0, max = numbers[0];
    while(i<size)
    {
        if(numbers[i] > max)
        {
            max = numbers[i];
        }
        ++i;
    }
    return max;
}