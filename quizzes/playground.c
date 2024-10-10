/* Explanation of the algorithm:
 * - The first while loop extracts elements from the `unsorted` stack one by one.
 * - The second while loop ensures that `temp` is inserted at the correct position in the `sorted` stack.
 * - Once all elements are in the `sorted` stack in descending order, they are transferred back to `unsorted` to get them in ascending order.
 *
 * Big-O Analysis:
 * Time Complexity: O(n^2)
 * - Each element must be pushed and popped multiple times to find its correct position.
 * - In the worst case, an element might require O(n) pushes and pops for n elements, resulting in O(n^2).
 *
 * Space Complexity: O(n)
 * - An additional stack of size n is used, which increases space usage to O(n).
 */


/* Function to sort an unsorted stack using an auxiliary stack */
void sort_stack(stack *unsorted) {
    stack sorted; // Auxiliary stack to hold sorted elements
    sorted.data = (int *)malloc(unsorted->capacity * sizeof(int));
    sorted.top = -1;
    sorted.capacity = unsorted->capacity;

    int temp; // Temporary variable to hold elements during sorting

    // Process all elements from the unsorted stack
    while (!is_empty(unsorted)) {
        // Step 1: Pop the top element from the unsorted stack
        temp = pop(unsorted);

        /* Step 2: Move elements from the sorted stack back to the unsorted stack
         * until we find the correct position for `temp`. This ensures that
         * all elements in `sorted` are in descending order.
         */
        while (!is_empty(&sorted) && peek(&sorted) > temp) {
            push(unsorted, pop(&sorted));
        }

        // Step 3: Push `temp` into the sorted stack at its correct position
        push(&sorted, temp);
    }

    /* At this point, the `sorted` stack has all elements in descending order.
     * To get them back in ascending order in the `unsorted` stack,
     * transfer elements back to `unsorted`.
     */
    while (!is_empty(&sorted)) {
        push(unsorted, pop(&sorted));
    }

    // Free memory allocated for the sorted stack
    free(sorted.data);
}


/* Explanation of the algorithm:
 * 1. `sort_stack_recursively` is called repeatedly until the stack is empty.
 * 2. Each recursive call pops the top element and holds it.
 * 3. Once the stack is fully empty, `insert_in_sorted_order` is used to reinsert elements in the correct order.
 *
 * `insert_in_sorted_order`:
 * - If the stack is empty or the top element is less than or equal to the current element (`value`), `value` is pushed onto the stack.
 * - Otherwise, it temporarily removes elements until the right position is found.
 * - After placing `value`, the temporarily removed elements are pushed back.
 *
 * Big-O Analysis:
 * Time Complexity: O(n^2)
 * - Each element is reinserted into a sorted stack one by one.
 * - For each insertion, elements may be moved within the stack multiple times, resulting in O(n) operations per element.
 *
 * Space Complexity: O(n)
 * - The additional space is due to the recursion stack for `sort_stack_recursively` and `insert_in_sorted_order`.
 */


/* Function to insert an element into a sorted stack */
void insert_in_sorted_order(stack *s, int value) {
    /* Base case: if stack is empty or the top element is less than or equal to `value`,
     * push `value` onto the stack.
     */
    if (is_empty(s) || peek(s) <= value) {
        push(s, value);
        return;
    }

    /* Recursive case: pop the top element and hold it */
    int temp = pop(s);

    /* Recursively call to find the right position for `value` */
    insert_in_sorted_order(s, value);

    /* Once `value` is placed, push the held element back */
    push(s, temp);
}

/* Function to sort the entire stack using recursion */
void sort_stack_recursively(stack *s) {
    /* Base case: if stack is empty, return */
    if (is_empty(s)) {
        return;
    }

    /* Pop the top element and hold it */
    int temp = pop(s);

    /* Recursively sort the remaining stack */
    sort_stack_recursively(s);

    /* Insert the popped element back into the sorted stack */
    insert_in_sorted_order(s, temp);
}


/**********************************************************************************************************************************************/
/* Define the buffer size */
#define BUFFER_SIZE 10

/* Buffer structure */
typedef struct {
    int *buffer;      // Array to hold buffer items
    int in;           // Index for next insertion
    int out;          // Index for next removal
} Buffer;

/* Shared resources */
Buffer shared_buffer;
pthread_mutex_t mutex; // Mutex lock for critical section
sem_t empty;           // Semaphore for empty slots
sem_t full;            // Semaphore for full slots

/* Producer thread function */
void *producer(void *arg) {
    int item;

    /* Produce items indefinitely */
    while (1) {
        /* Generate an item to be produced (e.g., a random number) */
        item = rand() % 100;

        /* Wait until there is an empty slot in the buffer */
        sem_wait(&empty);

        /* Lock the buffer to ensure mutual exclusion */
        pthread_mutex_lock(&mutex);

        /* Insert the item into the buffer */
        shared_buffer.buffer[shared_buffer.in] = item;
        shared_buffer.in = (shared_buffer.in + 1) % BUFFER_SIZE;

        /* Unlock the buffer */
        pthread_mutex_unlock(&mutex);

        /* Signal that there is one more full slot available */
        sem_post(&full);

        /* Sleep to simulate production time */
        sleep(1); // Replace with actual production logic as needed
    }

    return NULL;
}

/* Consumer thread function */
void *consumer(void *arg) {
    int item;

    /* Consume items indefinitely */
    while (1) {
        /* Wait until there is a full slot in the buffer */
        sem_wait(&full);

        /* Lock the buffer to ensure mutual exclusion */
        pthread_mutex_lock(&mutex);

        /* Remove an item from the buffer */
        item = shared_buffer.buffer[shared_buffer.out];
        shared_buffer.out = (shared_buffer.out + 1) % BUFFER_SIZE;

        /* Unlock the buffer */
        pthread_mutex_unlock(&mutex);

        /* Signal that there is one more empty slot available */
        sem_post(&empty);

        /* Sleep to simulate consumption time */
        sleep(1); // Replace with actual consumption logic as needed
    }

    return NULL;
}

/* Explanation:
 * - The producer waits until there is at least one empty slot (`sem_wait(&empty)`), locks the buffer (`pthread_mutex_lock`),
 *   and inserts an item at the `in` index. It then signals that there is one more full slot (`sem_post(&full)`).
 * - The consumer waits until there is at least one full slot (`sem_wait(&full)`), locks the buffer, and removes an item 
 *   at the `out` index. It then signals that there is one more empty slot (`sem_post(&empty)`).
 *
 * Synchronization Mechanisms:
 * - `sem_t empty`: Controls the number of empty slots. Producers wait until there's space to add items.
 * - `sem_t full`: Controls the number of full slots. Consumers wait until there are items to consume.
 * - `pthread_mutex_t mutex`: Ensures only one thread modifies the buffer at a time, preventing race conditions.
 */

/* Initialization function to set up shared resources */
void init_buffer(int size) {
    /* Allocate memory for the buffer */
    shared_buffer.buffer = (int *)malloc(size * sizeof(int));
    shared_buffer.in = 0;  // Initialize `in` index
    shared_buffer.out = 0; // Initialize `out` index

    /* Initialize the mutex */
    pthread_mutex_init(&mutex, NULL);

    /* Initialize semaphores */
    sem_init(&empty, 0, size); // Initial value of `empty` is the buffer size
    sem_init(&full, 0, 0);     // Initial value of `full` is 0 (no items in buffer)
}

/* Cleanup function to free resources */
void cleanup_buffer() {
    free(shared_buffer.buffer);
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
}

/* Big-O Analysis:
 * - Time Complexity: O(1) per operation (insert/remove).
 *   Each producer and consumer operation only involves constant-time operations on the buffer.
 * - Space Complexity: O(n), where n is the buffer size.
 *   The buffer itself is the primary data structure used for storage.
 */






int SwapBits(int idx1, int idx2, int num)
{
    int mask = ((num >> idx1) & 1) ^ ((num >> idx2) & 1); // 1 for diferent, 0 for the same.
    mask = (mask <<idx1) | (mask <<idx2); //either 11 if were different or 00 if were the same.
    return num ^ mask; // if were the same, XOR with 0 (0,0 will produce 0,  0,1 will produce 1)

}

int RotateRight(int idx, int num)
{
    idx = idx % 8;
    return (num >> idx) | (num << 8 - idx);
}

int RotateLeft(int idx, int num)
{
    idx = idx % 8;
    return (num << idx) | (num >> 8 - idx);
}

int BitMirror(int num)
{
    int ret = 0, i = 0, curr_bit = 0;
    int bits = sizeof(int) * 8;
    for(i=0; i < sizeof(int)*8; ++i)
    {   
        curr_bit = (num >> i) &1;
        ret = (curr_bit << bits-i-1) | ret; // must have the -1, can't shift 32-0, this will overflow.
    }
    return ret;
}


int BitMirror(int num)
{
    int ret = 0, i = 0, curr_bit = 0;
    int bits = sizeof(int) * 8;
    for(i=0; i bits; ++i)
    {   
        curr_bit = (num >> i) &1;
        ret = (curr_bit << bits-i-1) | ret; // must have the -1, can't shift 32-0, this will overflow.
    }
    return ret;
}

unsigned int ByteMirror(unsigned int num)
{
	num = ((num & 0x0000FFFF) << 16) | ((num & 0xFFFF0000) >> 16);
	num = ((num & 0x00FF00FF) << 8) | ((num & 0xFF00FF00) >> 8);
	num = ((num & 0x0F0F0F0F) << 4) | ((num & 0xF0F0F0F0) >> 4);
	num = ((num & 0x33333333) << 2) | ((num & 0xCCCCCCCC) >> 2);
	num = ((num & 0x55555555) << 1) | ((num & 0xAAAAAAAA) >> 1);

	return num;
}

unsigned char CharByteMirror(unsigned char num)
{
    // Initial value: num = 10101010 (0xAA in hex)

    // Step 1: Swap nibbles (4 bits)
    // High nibble:    1010
    // Low nibble:     1010
    // Result after this step: 
    // num = ((num & 0xF0) << 4) | ((num & 0x0F) >> 4);
    // num = (10101010 & 11110000) << 4  | (10101010 & 00001111) >> 4;
    // num = (10100000) | (00001010) 
    // num = 10100000 (0xA0)

    num = ((num & 0xF0) << 4) | ((num & 0x0F) >> 4);

    // Step 2: Swap pairs of bits (2 bits)
    // Before this step: num = 10100000 (0xA0)
    // Binary: 
    // num = ((num & 0x33) << 2) | ((num & 0xCC) >> 2);
    // num = (10100000 & 00110011) << 2  | (10100000 & 11001100) >> 2;
    // num = (00000000) | (00101000) 
    // num = 00101000 (0x28)

    num = ((num & 0x33) << 2) | ((num & 0xCC) >> 2);

    // Step 3: Swap individual bits (1 bit)
    // Before this step: num = 00101000 (0x28)
    // Binary: 
    // num = ((num & 0x55) << 1) | ((num & 0xAA) >> 1);
    // num = (00101000 & 01010101) << 1  | (00101000 & 10101010) >> 1;
    // num = (00000000) | (00010100) 
    // num = 01010100 (0x54)

    num = ((num & 0x55) << 1) | ((num & 0xAA) >> 1);

    return num; // Final value: 01010100 (0x54 in hex)
}




int CountSetBits(int num)
{
    int count = 0;
    while(num > 0)
    {
        num &= (num - 1);
        ++count;
    }
    return count;
}