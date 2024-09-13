#define _POSIX_C_SOURCE 200809L /*spinlock*/
#define _DEFAULT_SOURCE /*usleep*/
#include <stdio.h> /*prints*/
#include <stdlib.h> /*malloc*/
#include <pthread.h> /*threads*/
#include <unistd.h> /*usleep, sleep*/
#include <string.h> /*memcpy*/

#define BUFFER_SIZE 1
#define BUFFER_SIZE_MULTIPLE 10
#define NUM_PRODUCERS 3
#define NUM_CONSUMERS 3
#define TRUE 1

typedef struct node node_t;

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

node_t *head = NULL;
node_t *tail = NULL;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_spinlock_t spinlock;

void InitializeSpinLock()
{
    pthread_spin_init(&spinlock, PTHREAD_PROCESS_PRIVATE);
}

void DestroySpinLock()
{
    pthread_spin_destroy(&spinlock);
}

void AcquireSpinLock()
{
    pthread_spin_lock(&spinlock);
}

void ReleaseSpinLock()
{
    pthread_spin_unlock(&spinlock);
}

void *Producer(void *arg)
{
    data_t *data_item = (data_t *)arg;
    while (TRUE)
    {
        void *item_ptr = malloc(data_item->size);
        if (item_ptr == NULL)
            return NULL;

        memcpy(item_ptr, data_item->data, data_item->size);

        AcquireSpinLock();

        while (count == BUFFER_SIZE)
        {
            ReleaseSpinLock();
            usleep(1000);
            AcquireSpinLock();
        }

        buffer[0] = item_ptr;
        count++;

        ReleaseSpinLock();

        printf("Produced: %s\n", (char *)item_ptr);

        usleep(200000);
    }
    return NULL;
}

void *Consumer()
{
    void *item_ptr = NULL;
    while (TRUE)
    {
        AcquireSpinLock();

        while (count == 0)
        {
            ReleaseSpinLock();
            usleep(1000);
            AcquireSpinLock();
        }
        item_ptr = buffer[0];
        count--;

        ReleaseSpinLock();
        printf("Consumed: %s\n", (char *)item_ptr);

        free(item_ptr);

        usleep(200000);
    }
    return NULL;
}

void SpinLockFull()
{
    pthread_t producer_thread, consumer_thread;

    char *data = "Exercise One !";
    size_t data_size = strlen(data) + 1;
    data_t data_item;
    data_item.data = data;
    data_item.size = data_size;

    InitializeSpinLock();

    pthread_create(&producer_thread, NULL, Producer, &data_item);
    pthread_create(&consumer_thread, NULL, Consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    DestroySpinLock();
}


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

    pthread_mutex_lock(&mutex);
    if (head != NULL)
    {
        node_t *temp = head;
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

void *ProducerEx2(void *arg)
{
    int data = 0, i = 0;
    int id = *(int *)arg;
    for (i = 0; i < 10; i++)
    {
        data = id * 100 + i;
        AddToList(data);
        printf("Producer %d produced %d\n", id, data);
        sleep(1);
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
        }
        else
        {
            sleep(1);
        }
    } while (TRUE);
    return NULL;
}

void OneMutex()
{
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];
    int producer_ids[NUM_PRODUCERS];
    int consumer_ids[NUM_CONSUMERS];
    int i = 0;

    for (i = 0; i < NUM_PRODUCERS; i++)
    {
        producer_ids[i] = i + 1;
        pthread_create(&producers[i], NULL, ProducerEx2, (void *)&producer_ids[i]);
    }

    for (i = 0; i < NUM_CONSUMERS; i++)
    {
        consumer_ids[i] = i + 1;
        pthread_create(&consumers[i], NULL, ConsumerEx2, (void *)&consumer_ids[i]);
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

    pthread_mutex_destroy(&mutex);
}


int main()
{
    /*SpinLockFull();*/
    OneMutex();
    return 0;
}
