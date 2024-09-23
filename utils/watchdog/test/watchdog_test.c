/* watchdog_test.c */
#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include <sys/wait.h>
#include "watchdog.h"

#define TEST_THRESHOLD 3
#define TEST_INTERVAL 2
#define SUCCESS 0
#define FAIL -1
#define MAX_WAIT_TIME 5

static volatile sig_atomic_t alarm_triggered = 0;

static void alarm_handler(int signum) {
    (void)signum;
    alarm_triggered = 1;
}

static void test_WDStart_WDStop(void) {
    int result, i;
    struct sigaction sa;

    result = WDStart(TEST_THRESHOLD, TEST_INTERVAL, 0, NULL);
    if (result != SUCCESS) return;

    sleep(1);

    WDStop();

    printf("WDStart and WDStop test completed.\n");
}
static void test_watchdog_restart(void) {
    pid_t pid;
    int status;
    int result;

    printf("Testing watchdog restart functionality...\n");

    pid = fork();
    if (pid == 0) {
        /* Child process */
        result = WDStart(TEST_THRESHOLD, TEST_INTERVAL, 0, NULL);
        assert(result == SUCCESS);
        sleep(TEST_THRESHOLD * TEST_INTERVAL + 1);
        /* This should not be reached if the watchdog works correctly */
        exit(1);
    } else if (pid > 0) {
        /* Parent process */
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGTERM) {
            printf("Watchdog restart test passed.\n");
        } else {
            printf("Watchdog restart test failed.\n");
        }
    } else {
        perror("fork");
        exit(1);
    }
}

int main(void) {
    printf("Starting Watchdog Test Suite\n");

    printf("Running test_WDStart_WDStop\n");
    test_WDStart_WDStop();
    printf("test_WDStart_WDStop completed\n");

    sleep(2);

    printf("Running test_watchdog_restart\n");
    test_watchdog_restart();
    printf("test_watchdog_restart completed\n");

    printf("All tests completed.\n");
    return 0;
}
