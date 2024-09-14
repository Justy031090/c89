#define _POSIX_C_SOURCE 200112L /* spinlock */
#define _DEFAULT_SOURCE         /* usleep */
#include <stdio.h>              /* prints */
#include <stdlib.h>             /* malloc */
#include <pthread.h>            /* threads */
#include <unistd.h>             /* usleep, sleep */
#include <string.h>             /* memcpy */
#include <semaphore.h>          /* semaphores */

#define BUFFER_SIZE 1
#define BUFFER_SIZE_MULTIPLE 10
#define NUM_PRODUCERS 3
#define NUM_CONSUMERS 3
#define TRUE 1
#define PSHARED_NUM 0

typedef void *(*thread_func_t)(void *);

typedef struct
{
    int *buffer;
    int head_idx;
    int tail_idx;
    pthread_mutex_t mutex_in;
    pthread_mutex_t mutex_out;
    sem_t empty_count;
    sem_t full_count;
} queue_t;

typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    sem_t sem;
    int pending_messages;
    int producer_status;
    int current_message;
} barrier_t;

void *Producer(void *arg);
void *Consumer();
void *ProducerEx2(void *arg);
void *ConsumerEx2(void *arg);
void *ProducerEx3(void *arg);
void *ConsumerEx3(void *arg);
void *ProducerEx5(void *arg);
void *ConsumerEx5(void *arg);
void *ProducerEx6();
void *ConsumerEx6(void *arg);

void CreateThreads(thread_func_t producer_func, thread_func_t consumer_func);
queue_t *CreateQ(size_t size);
void DestroyQ(queue_t *q);
void EnqueueEx5(queue_t *q, int data);
int DequeueEx5(queue_t *q);
void CreateThreadEx5(queue_t *q, thread_func_t producer_func, thread_func_t consumer_func);
void InitBarrier(barrier_t *barrier);
void DestroyBarrier(barrier_t *barrier);
void AddToList(int data);
int RemoveFromList(int *data);
int AddToQueue(int data);
int RemoveFromQueue();

/* Global variables */
void *buffer[BUFFER_SIZE];
int count = 0;
barrier_t barrier;
sem_t semaphore;

typedef struct node node_t;

struct node
{
    void *data;
    node_t *next;
};

typedef struct
{
    void *data;
    size_t size;
} data_t;

node_t *head = NULL;
node_t *tail = NULL;

int queue[BUFFER_SIZE_MULTIPLE];
int head_idx = 0, tail_idx = 0;
sem_t empty_count;
sem_t full_count;

pthread_spinlock_t spinlock;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t semaphore_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Spinlock producer and consumer */
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
    void *item_ptr;
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

/* Linked list producer and consumer */
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
    while (TRUE)
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
    }
    return NULL;
}

/* Fixed-size queue with one mutex and one semaphore */
void *ProducerEx3(void *arg)
{
    int id = *(int *)arg;
    int i = 0, data = 0;
    for (i = 0; i < 10; i++)
    {
        data = id * 100 + i;

        sem_wait(&empty_count);
        pthread_mutex_lock(&mutex);

        AddToQueue(data);
        printf("Producer %d produced %d\n", id, data);

        pthread_mutex_unlock(&mutex);
        sem_post(&full_count);

        usleep(2000);
    }
    return NULL;
}

void *ConsumerEx3(void *arg)
{
    int id = *(int *)arg;
    int data = 0;
    while (TRUE)
    {
        sem_wait(&full_count);
        pthread_mutex_lock(&mutex);
        data = RemoveFromQueue();

        printf("Consumer %d consumed %d\n", id, data);

        pthread_mutex_unlock(&mutex);
        sem_post(&empty_count);

        usleep(2000);
    }
    return NULL;
}

/* Fixed-size queue with two mutexes and two semaphores */
void *ProducerEx5(void *arg)
{
    queue_t *queue = (queue_t *)arg;
    int i = 0;
    for (i = 0; i < 20; i++)
    {
        EnqueueEx5(queue, i);
        printf("Produced %d\n", i);
        usleep(2000);
    }
    return NULL;
}

void *ConsumerEx5(void *arg)
{
    queue_t *queue = (queue_t *)arg;
    int data = 0;
    while (TRUE)
    {
        data = DequeueEx5(queue);
        printf("Consumed %d\n", data);
        usleep(20000);
    }
    return NULL;
}

/* Barrier with one condition variable, one mutex, and one semaphore */
void *ProducerEx6(void *arg)
{
    int i;
    int j;
    for (i = 0; i < 10; i++)
    {
        pthread_mutex_lock(&barrier.mutex);

        while (barrier.pending_messages > 0)
        {
            pthread_cond_wait(&barrier.cond, &barrier.mutex);
        }

        barrier.current_message = i;
        barrier.pending_messages = NUM_CONSUMERS;

        printf("Producer produced message %d\n", i);

        pthread_mutex_unlock(&barrier.mutex);

        for (j = 0; j < NUM_CONSUMERS; j++)
        {
            sem_post(&barrier.sem);
        }

        usleep(1000);
    }

    pthread_mutex_lock(&barrier.mutex);
    barrier.producer_status = 1;
    pthread_cond_broadcast(&barrier.cond);
    pthread_mutex_unlock(&barrier.mutex);

    for (j = 0; j < NUM_CONSUMERS; j++)
    {
        sem_post(&barrier.sem);
    }

    return NULL;
}

void *ConsumerEx6(void *arg)
{
    long id = (long)arg;
    int message;
    while (TRUE)
    {
        sem_wait(&barrier.sem);

        pthread_mutex_lock(&barrier.mutex);

        if (barrier.producer_status == 1 && barrier.pending_messages == 0)
        {
            pthread_mutex_unlock(&barrier.mutex);
            break;
        }

        printf("Consumer %ld consumed message %d\n", id, barrier.current_message);

        message = barrier.current_message;
        barrier.pending_messages--;

        if (barrier.pending_messages == 0)
        {
            pthread_cond_signal(&barrier.cond);
        }

        pthread_mutex_unlock(&barrier.mutex);

        usleep(2000);
    }

    return NULL;
}

/* Helper functions */
void AddToList(int data)
{
    int *data_ptr = NULL;
    node_t *new_node = malloc(sizeof(node_t));
    if (!new_node)
        return;

    data_ptr = malloc(sizeof(int));
    if (!data_ptr)
    {
        free(new_node);
        return;
    }
    *data_ptr = data;

    new_node->data = data_ptr;
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
        *data = *((int *)temp->data);
        head = head->next;
        if (head == NULL)
        {
            tail = NULL;
        }
        free(temp->data);
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

queue_t *CreateQ(size_t size)
{
    queue_t *q = malloc(sizeof(queue_t));
    if (NULL == q)
        return NULL;

    q->buffer = malloc(size * sizeof(int));
    if (NULL == q->buffer)
        return NULL;

    q->head_idx = 0;
    q->tail_idx = 0;
    sem_init(&q->empty_count, PSHARED_NUM, size);
    sem_init(&q->full_count, PSHARED_NUM, 0);
    pthread_mutex_init(&q->mutex_in, NULL);
    pthread_mutex_init(&q->mutex_out, NULL);

    return q;
}

void DestroyQ(queue_t *q)
{
    if (!q)
        return;

    free(q->buffer);
    sem_destroy(&q->empty_count);
    sem_destroy(&q->full_count);
    pthread_mutex_destroy(&q->mutex_in);
    pthread_mutex_destroy(&q->mutex_out);
    free(q);
}

int DequeueEx5(queue_t *q)
{
    int data;
    sem_wait(&q->full_count);
    pthread_mutex_lock(&q->mutex_out);

    data = q->buffer[q->head_idx];
    q->head_idx = (q->head_idx + 1) % BUFFER_SIZE_MULTIPLE;

    pthread_mutex_unlock(&q->mutex_out);
    sem_post(&q->empty_count);

    return data;
}

void EnqueueEx5(queue_t *q, int data)
{
    sem_wait(&q->empty_count);
    pthread_mutex_lock(&q->mutex_in);

    q->buffer[q->tail_idx] = data;
    q->tail_idx = (q->tail_idx + 1) % BUFFER_SIZE_MULTIPLE;

    pthread_mutex_unlock(&q->mutex_in);
    sem_post(&q->full_count);
}

void CreateThreadEx5(queue_t *q, thread_func_t producer_func, thread_func_t consumer_func)
{
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];
    int i = 0;

    for (i = 0; i < NUM_PRODUCERS; i++)
    {
        pthread_create(&producers[i], NULL, producer_func, q);
    }

    for (i = 0; i < NUM_CONSUMERS; i++)
    {
        pthread_create(&consumers[i], NULL, consumer_func, q);
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

void InitBarrier(barrier_t *barrier)
{
    pthread_mutex_init(&barrier->mutex, NULL);
    pthread_cond_init(&barrier->cond, NULL);
    sem_init(&barrier->sem, PSHARED_NUM, 0);
    barrier->pending_messages = 0;
    barrier->producer_status = 0;
    barrier->current_message = -1;
}

void DestroyBarrier(barrier_t *barrier)
{
    pthread_mutex_destroy(&barrier->mutex);
    pthread_cond_destroy(&barrier->cond);
    sem_destroy(&barrier->sem);
}

/* Testing functions */
void SpinLockFull()
{
    pthread_spin_init(&spinlock, PTHREAD_PROCESS_PRIVATE);
    CreateThreads(Producer, Consumer);
    pthread_spin_destroy(&spinlock);
}

void OneMutex()
{
    pthread_mutex_destroy(&mutex);
    CreateThreads(ProducerEx3, ConsumerEx3);
}

void OneMutexOneSemaphore()
{
    sem_init(&semaphore, PSHARED_NUM, BUFFER_SIZE);
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

void TwoMutexesTwoSems()
{
    queue_t queue;
    pthread_t producer, consumer;
    queue.head_idx = 0;
    queue.tail_idx = 0;
    pthread_mutex_init(&queue.mutex_in, NULL);
    pthread_mutex_init(&queue.mutex_out, NULL);
    sem_init(&queue.empty_count, 0, BUFFER_SIZE);
    sem_init(&queue.full_count, 0, 0);

    pthread_create(&producer, NULL, ProducerEx5, &queue);
    pthread_create(&consumer, NULL, ConsumerEx5, &queue);

    pthread_join(producer, NULL);
    pthread_cancel(consumer);
    pthread_join(consumer, NULL);

    pthread_mutex_destroy(&queue.mutex_in);
    pthread_mutex_destroy(&queue.mutex_out);
    sem_destroy(&queue.empty_count);
    sem_destroy(&queue.full_count);
}

void Barrier()
{
    InitBarrier(&barrier);
    CreateThreads(ProducerEx6, ConsumerEx6);
    DestroyBarrier(&barrier);
}

int main()
{
    /*
    SpinLockFull();
    OneMutex();
    OneMutexOneSemaphore();
    OneMutexTwoSems();
    TwoMutexesTwoSems();
     */
    Barrier();
    return 0;
}
