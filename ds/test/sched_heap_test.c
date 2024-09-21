#include <stdio.h>
#include "sched_heap.h"

typedef struct params {
    int a;
    sd_t *sd;
} p_t;

time_t printfunc2(void *params) {
    static int p = 0;
    if (4 == p) {
        p = 0;
        SCHEDStop(((p_t *)params)->sd);
    }
    printf("%d\n", (p + ((p_t *)params)->a));
    ++p;
    return 1;
}

time_t printfunc1(void *params) {
    static int i = 0;
    if (3 == i) {
        i = 0;
        return 0;
    }
    printf("%d\n", (i + ((p_t *)params)->a + 3));
    ++i;
    return 1;
}

static void CleanUp(void *params) {
    (void)params;
}

int main() {
    sd_t *new_sd = SCHEDCreate();
    p_t params1;
    my_uid_t task1_id; 
    my_uid_t task2_id;
    params1.a = 1;
    params1.sd = new_sd;

    if (1 == SCHEDIsEmpty(new_sd)) {
        printf("IsEmpty: empty success\n");
    } else {
        printf("IsEmpty: empty FAIL\n");
    }

    task1_id = SCHEDAddTask(new_sd, time(NULL) + 1, printfunc1, (void *)&params1, CleanUp, (void *)&params1);
    task2_id = SCHEDAddTask(new_sd, time(NULL) + 2, printfunc2, (void *)&params1, CleanUp, (void *)&params1);

    if (0 == SCHEDIsEmpty(new_sd)) {
        printf("IsEmpty: full success\n");
    } else {
        printf("IsEmpty: empty FAIL\n");
    }

    if (2 == SCHEDSize(new_sd)) {
        printf("Size success\n");
    } else {
        printf("Size FAIL\n");
    }

    SCHEDRun(new_sd);
    printf("if printed 14523... then size success & run1 success & stop success\n");

    task1_id = SCHEDAddTask(new_sd, time(NULL) + 1, printfunc1, (void *)&params1, CleanUp, (void *)&params1);
    SCHEDRemoveTask(task2_id, new_sd);
    
    SCHEDRun(new_sd);
    
    SCHEDAddTask(new_sd, time(NULL) + 1, printfunc1, (void *)&params1, CleanUp, (void *)&params1);
    SCHEDAddTask(new_sd, time(NULL) + 2, printfunc1, (void *)&params1, CleanUp, (void *)&params1);
    SCHEDAddTask(new_sd, time(NULL) + 3, printfunc1, (void *)&params1, CleanUp, (void *)&params1);

    SCHEDClear(new_sd);
    if (SCHEDIsEmpty(new_sd)) {
        printf("Clear success\n");
    } else {
        printf("Clear FAIL\n");
    }

    SCHEDDestroy(new_sd);
    return 0;
}
