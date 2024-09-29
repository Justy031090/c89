#define _DEFAULT_SOURCE /* usleep */
#include <stdio.h> /* printf, fprintf */
#include <stdlib.h> /* atoi, getenv */
#include <unistd.h> /* sleep */
#include <assert.h> /* assert */
#include <signal.h> /* signal handling */
#include <sys/wait.h> /* waitpid */
#include <string.h> /* strcmp */
#include <sys/time.h> /* time */
#include <errno.h> /* errno */

#include "watchdog.h"
#include "watchdog_combined.h"

#define TEST_TIMEOUT 30
#define TEST_THRESHOLD 3
#define TEST_INTERVAL 1
#define TEST_DURATION 5

static void test_StartStop(void)
{
    int result;
    pid_t pid = getpid();
    char *wd_running;
    char *process_type;

    printf("[Process %d] Testing WDStart and WDStop...\n", pid);

    result = WDStart(TEST_THRESHOLD, TEST_INTERVAL, 0, NULL);
    printf("[Process %d] WDStart result: %d\n", pid, result);
    assert(result == SUCCESS);
    
    wd_running = getenv(ENV_WD_RUNNING);
    process_type = getenv(ENV_PROCESS_TYPE);
    
    printf("[Process %d] ENV_WD_RUNNING: %s\n", pid, wd_running ? wd_running : "NULL");
    printf("[Process %d] ENV_PROCESS_TYPE: %s\n", pid, process_type ? process_type : "NULL");
    
    assert(wd_running && strcmp(wd_running, "1") == 0);
    assert(process_type && strcmp(process_type, PROCESS_TYPE_CLIENT) == 0);
    
    printf("[Process %d] Sleeping for %d seconds\n", pid, TEST_DURATION);
    sleep(TEST_DURATION);
    
    printf("[Process %d] Calling WDStop\n", pid);
    result = WDStop();
    printf("[Process %d] WDStop result: %d\n", pid, result);
    assert(result == SUCCESS);
    
    wd_running = getenv(ENV_WD_RUNNING);
    process_type = getenv(ENV_PROCESS_TYPE);
    
    printf("[Process %d] After WDStop - ENV_WD_RUNNING: %s\n", pid, wd_running ? wd_running : "NULL");
    printf("[Process %d] After WDStop - ENV_PROCESS_TYPE: %s\n", pid, process_type ? process_type : "NULL");
    
    assert(getenv(ENV_WD_RUNNING) == NULL);
    assert(getenv(ENV_PROCESS_TYPE) == NULL);

    printf("[Process %d] WDStart and WDStop test passed.\n", pid);
}

static void test_Revival(void)
{
    int result;
    pid_t pid;
    int status;
    int i;

    printf("Testing mutual revival...\n");

    pid = fork();
    if (pid == 0)
    {
        /* Child process */
        result = WDStart(TEST_THRESHOLD, TEST_INTERVAL, 0, NULL);
        if (result != SUCCESS)
        {
            fprintf(stderr, "Failed to start watchdog: %d\n", result);
            exit(EXIT_FAILURE);
        }
        
        printf("Child process: Watchdog started\n");
        
        /* Simulate work and crashes */
        for (i = 0; i < 3; i++)
        {
            printf("Child process running (iteration %d)...\n", i + 1);
            sleep(TEST_THRESHOLD * TEST_INTERVAL * 2);
            
            if (i < 2)
            {
                printf("Child process simulating crash...\n");
                raise(SIGTERM);  /* Simulate a crash */
                /* If we reach here, it means the process was revived */
                printf("Child process revived after crash\n");
            }
        }
        
        /* Stop the watchdog */
        result = WDStop();
        if (result != SUCCESS)
        {
            fprintf(stderr, "Failed to stop watchdog: %d\n", result);
            exit(EXIT_FAILURE);
        }
        
        printf("Child process: Watchdog stopped successfully\n");
        exit(EXIT_SUCCESS);
    }
    else if (pid > 0)
    {
        /* Parent process */
        /* Give child process time to start */
        sleep(1); 
        
        /* Wait for child process to exit */
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status))
        {
            printf("Child process exited with status: %d\n", WEXITSTATUS(status));
            assert(WEXITSTATUS(status) == EXIT_SUCCESS);
        }
        else if (WIFSIGNALED(status))
        {
            printf("Child process terminated by signal: %d\n", WTERMSIG(status));
            assert(WTERMSIG(status) != SIGKILL && "Child process should not be terminated by SIGKILL");
        }
        else
        {
            printf("Unexpected child process termination\n");
            assert(FAIL && "Unexpected child process termination");
        }
        
        printf("Mutual revival test passed.\n");
    }
    else
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
}


static void test_Termination(void)
{
    int result;
    pid_t pid;
    int status;
    int i;
    /* Time tracking for timeout */
    struct timeval start, now;
    double elapsed;

    printf("Testing watchdog termination...\n");

    /* Get start time */
    gettimeofday(&start, NULL);

    pid = fork();
    if (pid == 0)
    {
        /* Child process */
        result = WDStart(TEST_THRESHOLD, TEST_INTERVAL, 0, NULL);
        if (result != SUCCESS)
        {
            fprintf(stderr, "Failed to start watchdog: %d\n", result);
            exit(EXIT_FAILURE);
        }
        
        printf("Child process: Watchdog started\n");
        
        /* Simulate some work */
        for (i = 0; i < TEST_THRESHOLD * 2; i++)
        {
            sleep(TEST_INTERVAL);
            printf("Child process still running... (iteration %d)\n", i + 1);
        }
        
        printf("Child process: Attempting to stop watchdog\n");
        /* Stop the watchdog */
        result = WDStop();
        if (result != SUCCESS)
        {
            fprintf(stderr, "Failed to stop watchdog: %d\n", result);
            exit(EXIT_FAILURE);
        }
        
        printf("Child process: Watchdog stopped successfully\n");
        exit(EXIT_SUCCESS);
    }
    else if (pid > 0)
    {
        /* Parent process */
        while (TRUE)
        {
            /* Get current time */
            gettimeofday(&now, NULL);
            elapsed = (now.tv_sec - start.tv_sec) + (now.tv_usec - start.tv_usec) / 1000000.0;
            
            if (elapsed > TEST_TIMEOUT)
            {
                printf("Test timed out after %.2f seconds\n", elapsed);
                kill(pid, SIGKILL); /* Kill the child process if timeout occurs */
                kill(pid, SIGKILL);
                waitpid(pid, &status, 0);
                assert(0 && "Test timed out");
                return;
            }
            
            /*WNOHANG Blocks Waiting*/
            if (waitpid(pid, &status, WNOHANG) != 0)
            {
                /* Break if child process has exited */
                break;
            }
            
            usleep(100000); 
        }
        /* Check the exit status of the child process */
        if (WIFEXITED(status))
        {
            printf("Child process exited with status: %d\n", WEXITSTATUS(status));
            assert(WEXITSTATUS(status) == EXIT_SUCCESS);
        }
        else if (WIFSIGNALED(status))
        {
            printf("Child process terminated by signal: %d\n", WTERMSIG(status));
            assert(0 && "Child process should not be terminated by a signal");
        }
        else
        {
            printf("Unexpected child process termination\n");
            assert(0 && "Unexpected child process termination");
        }
        
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

    test_StartStop();
    sleep(1);

    test_Termination();
    sleep(1);

    test_Revival();

    printf("All tests completed successfully.\n");
    return 0;
}