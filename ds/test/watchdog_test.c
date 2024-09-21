#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>


#include "watchdog.h"
#include "watchdog_combined.h"

void test_InitializeSharedMem() {
    process_args_t *shared_args = NULL;
    int shm_id;
    size_t threshold = 10, interval = 2;
    char *argv[] = {"test_program", "arg1", "arg2"};
    int result = InitializeSharedMem(&shared_args, &shm_id, threshold, interval, 3, argv);
    sd_t *sched = SCHEDCreate();

    assert(result == SUCCESS);
    assert(shared_args != NULL);
    assert(shared_args->threshold == threshold);
    assert(shared_args->interval == interval);
    assert(shared_args->argc == 3);
    assert(strcmp(shared_args->argv[0], "arg1") == 0);
    assert(strcmp(shared_args->argv[1], "arg2") == 0);


    CleanupEverything(sched, shared_args, shm_id, 0);
}

void test_InitializeSemaphore() {
    int sem_id;
    int result = InitializeSemaphore(&sem_id);
    
    assert(result == SUCCESS);
    assert(sem_id >= 0);


    if (semctl(sem_id, 0, IPC_RMID) == -1) {
        perror("Failed to remove semaphore set");
    }
}

void test_WDStart() {
    size_t threshold = 1, interval = 1;
    char *argv[] = {"./test_program", NULL};
    
    int result = WDStart(threshold, interval, 1, argv);
    assert(result == 0);
    sleep(2);

    WDStop();
}

void test_WDStop() {
    size_t threshold = 1, interval = 1;
    char *argv[] = {"./test_program", NULL};

    WDStart(threshold, interval, 1, argv);

    WDStop();
}

void test_SendSignal() {

}

void test_CheckCounter() {

}

int main() {
    printf("Running tests...\n");

    test_InitializeSharedMem();
    printf("test_InitializeSharedMem passed.\n");

    test_InitializeSemaphore();
    printf("test_InitializeSemaphore passed.\n");

    test_WDStart();
    printf("test_WDStart passed.\n");

    test_WDStop();
    printf("test_WDStop passed.\n");

    return 0;
}
