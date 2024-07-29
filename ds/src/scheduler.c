
struct scheduler
{
	p_q_t *pq;
}

sd_t *SDCreate();

void SDDestroy(sd_t *sd);

int SDAddTask(sd_t *sd, time_t exe_time, void *func(void *params, sd_t *sd, task_t *task), void *params, void CleanUp()/* func pointer*/);

/*void SDRemoveTask(task_t *task, sd_t *sd);
*/
int SDRun(sd_t *sd);

int SDIsEmpty(const sd_t *sd);

size_t SDSize(const sd_t *sd);

void SDClear(sd_t *sd);
