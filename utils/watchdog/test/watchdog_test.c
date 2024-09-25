#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

#include "watchdog.h"
#include "watchdog_combined.h"

#define TEST_THRESHOLD 3
#define TEST_INTERVAL 1
#define TEST_DURATION 5

static void test_WDStart_WDStop(void)
{
    int result;

    printf("Testing WDStart and WDStop...\n");

    result = WDStart(TEST_THRESHOLD, TEST_INTERVAL, 0, NULL);
    assert(result == SUCCESS);
    
    assert(strcmp(getenv(ENV_WD_RUNNING), "1") == 0);
    assert(strcmp(getenv(ENV_PROCESS_TYPE), PROCESS_TYPE_CLIENT) == 0);
    
    sleep(TEST_DURATION);
    
    result = WDStop();
    assert(result == SUCCESS);
    assert(getenv(ENV_WD_RUNNING) == NULL);
    assert(getenv(ENV_PROCESS_TYPE) == NULL);

    printf("WDStart and WDStop test passed.\n");
}

static void test_watchdog_termination(void)
{
    int result;
    pid_t pid;
    int status;

    printf("Testing watchdog termination...\n");

    pid = fork();
    if (pid == 0)
    {
        result = WDStart(TEST_THRESHOLD, TEST_INTERVAL, 0, NULL);
        assert(result == SUCCESS);
        
        sleep(TEST_THRESHOLD * TEST_INTERVAL * 2);
        
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        waitpid(pid, &status, 0);
        assert(WIFSIGNALED(status) && WTERMSIG(status) == SIG_STOP);
        printf("Watchdog termination test passed.\n");
    }
    else
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
}

int main()
{
    printf("Starting Watchdog Test Suite\n");

    test_WDStart_WDStop();
    sleep(1);

    test_watchdog_termination();

    printf("All tests completed successfully.\n");
    return 0;
}