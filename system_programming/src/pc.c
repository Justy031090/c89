#define _POSIX_C_SOURCE 200809L /*spinlock*/
#define _DEFAULT_SOURCE         /*usleep*/
#include <stdio.h>              /*prints*/
#include <stdlib.h>             /*malloc*/
#include <pthread.h>            /*threads*/
#include <unistd.h>             /*usleep, sleep*/
#include <string.h>             /*memcpy*/
#include <semaphore.h>          /*semaphores*/

#define BUFFER_SIZE 1
#define BUFFER_SIZE_MULTIPLE 10
#define NUM_PRODUCERS 3
#define NUM_CONSUMERS 3
#define TRUE 1
#define PSHARED_NUM 0

typedef struct node node_t;
typedef void *(*thread_func_t)(void *);

void *buffer[BUFFER_SIZE];
int count = 0;

typedef struct
{
    void *data;
    size_t size;
} data_t;

struct node
{
    int data;
    node_t *next;
};

sem_t semaphore;
pthread_spinlock_t spinlock;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/*linked list variables*/
node_t *head = NULL;
node_t *tail = NULL;

/*queue variables*/
int queue[BUFFER_SIZE_MULTIPLE];
int head_idx = 0, tail_idx = 0;
sem_t empty_count;
sem_t full_count;

void CreateThreads(thread_func_t producer_func, thread_func_t consumer_func);

void AddToList(int data)
{
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    new_node->data = data;
    new_node->next = NULL;

    pthread_mutex_lock(&mutex);
    if (tail == NULL)
    {
        head = new_node;
        tail = new_node;
    }
    else
    {
        tail->next = new_node;
        tail = new_node;
    }
    pthread_mutex_unlock(&mutex);
}

int RemoveFromList(int *data)
{
    int item_available = 0;
    node_t *temp = NULL;

    pthread_mutex_lock(&mutex);
    if (head != NULL)
    {
        temp = head;
        *data = temp->data;
        head = head->next;
        if (head == NULL)
        {
            tail = NULL;
        }
        free(temp);
        item_available = 1;
    }
    pthread_mutex_unlock(&mutex);
    return item_available;
}

int AddToQueue(int data)
{
    queue[tail_idx] = data;
    tail_idx = (tail_idx + 1) % BUFFER_SIZE_MULTIPLE;
    return 0;
}

int RemoveFromQueue()
{
    int data = queue[head_idx];
    head_idx = (head_idx + 1) % BUFFER_SIZE_MULTIPLE;
    return data;
}

void *Producer(void *arg)
{
    data_t *data_item = (data_t *)arg;
    void *item_ptr = NULL;
    while (TRUE)
    {
        item_ptr = malloc(data_item->size);
        if (item_ptr == NULL)
            return NULL;

        memcpy(item_ptr, data_item->data, data_item->size);

        pthread_spin_lock(&spinlock);

        while (count == BUFFER_SIZE)
        {
            pthread_spin_unlock(&spinlock);
            usleep(1000);
            pthread_spin_lock(&spinlock);
        }

        buffer[0] = item_ptr;
        count++;

        pthread_spin_unlock(&spinlock);

        printf("Produced: %s\n", (char *)item_ptr);

        usleep(2000);
    }
    return NULL;
}

void *Consumer()
{
    void *item_ptr = NULL;
    while (TRUE)
    {
        pthread_spin_lock(&spinlock);

        while (count == 0)
        {
            pthread_spin_unlock(&spinlock);
            usleep(1000);
            pthread_spin_lock(&spinlock);
        }
        item_ptr = buffer[0];
        count--;

        pthread_spin_unlock(&spinlock);
        printf("Consumed: %s\n", (char *)item_ptr);

        free(item_ptr);

        usleep(200000);
    }
    return NULL;
}

void *ProducerEx2(void *arg)
{
    int data = 0, i = 0;
    int id = *(int *)arg;
    for (i = 0; i < 10; i++)
    {
        data = id * 100 + i;
        sem_wait(&semaphore);

        AddToList(data);
        printf("Producer %d produced %d\n", id, data);

        usleep(2000);
    }
    return NULL;
}

void *ConsumerEx2(void *arg)
{
    int id = *(int *)arg;
    int data = 0;
    do
    {
        if (RemoveFromList(&data))
        {
            printf("Consumer %d consumed %d\n", id, data);
            sem_post(&semaphore);
        }
        else
        {
            usleep(2000);
        }
    } while (TRUE);

    return NULL;
}

void *ProducerEx3(void *arg)
{
    int id = *(int *)arg;
    int i = 0, item = 0;
    for (i = 0; i < 10; i++)
    {
        item = id * 100 + i;

        sem_wait(&empty_count);
        pthread_mutex_lock(&mutex);

        AddToQueue(item);
        printf("Producer %d produced %d\n", id, item);

        pthread_mutex_unlock(&mutex);
        sem_post(&full_count);

        usleep(2000);
    }

    return NULL;
}

void *ConsumerEx3(void *arg)
{
    int id = *(int *)arg;
    int item = 0;

    while (TRUE)
    {
        sem_wait(&full_count);
        pthread_mutex_lock(&mutex);
        item = RemoveFromQueue();

        printf("Consumer %d consumed %d\n", id, item);

        pthread_mutex_unlock(&mutex);
        sem_post(&empty_count);

        usleep(2000);
    }

    return NULL;
}

void CreateThreads(thread_func_t producer_func, thread_func_t consumer_func)
{
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];
    int producer_ids[NUM_PRODUCERS];
    int consumer_ids[NUM_CONSUMERS];
    int i = 0;

    for (i = 0; i < NUM_PRODUCERS; i++)
    {
        producer_ids[i] = i + 1;
    }

    for (i = 0; i < NUM_CONSUMERS; i++)
    {
        consumer_ids[i] = i + 1;
    }

    for (i = 0; i < NUM_PRODUCERS; i++)
    {
        pthread_create(&producers[i], NULL, producer_func, &producer_ids[i]);
    }

    for (i = 0; i < NUM_CONSUMERS; i++)
    {
        pthread_create(&consumers[i], NULL, consumer_func, &consumer_ids[i]);
    }

    for (i = 0; i < NUM_PRODUCERS; i++)
    {
        pthread_join(producers[i], NULL);
    }

    for (i = 0; i < NUM_CONSUMERS; i++)
    {
        pthread_cancel(consumers[i]);
    }

    for (i = 0; i < NUM_CONSUMERS; i++)
    {
        pthread_join(consumers[i], NULL);
    }
}

void OneMutex()
{
    pthread_mutex_destroy(&mutex);
    CreateThreads(ProducerEx3, ConsumerEx3);
}

void OneMutexOneSemaphore()
{
    sem_init(&semaphore, PSHARED_NUM, BUFFER_SIZE_MULTIPLE);
    CreateThreads(ProducerEx2, ConsumerEx2);
    sem_destroy(&semaphore);
    pthread_mutex_destroy(&mutex);
}

void OneMutexTwoSems()
{
    sem_init(&empty_count, PSHARED_NUM, BUFFER_SIZE);
    sem_init(&full_count, PSHARED_NUM, 0);
    CreateThreads(ProducerEx3, ConsumerEx3);
    sem_destroy(&empty_count);
    sem_destroy(&full_count);
    pthread_mutex_destroy(&mutex);
}

int main()
{

    /* SpinLockFull();
     OneMutex();
     OneMutexTwoSems();
    */
    OneMutexOneSemaphore();
    return 0;
}
