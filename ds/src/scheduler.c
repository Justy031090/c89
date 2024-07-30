
struct scheduler
{
	p_q_t *pq;
	int stop;
}

static int MatchUID(my_uid_t uid1, my_uid_t uid2);

typedef struct scheduler sd_t;

int SCHEDRun(sd_t *sd)
{
	int IsEmpty = PQIsEmpty(sd->pq);
	void *task_scheduled = NULL;
	
	while(!IsEmpty && !sd->stop)
	{
		task_scheduled = PQPeek(sd->pq);
		task_scheduled->func(task_scheduled->params)
		task_scheduled->clean_up()
	}
}

my_uid_t SCHEDAddTask(sd_t *sd, time_t exe_time, func_t func, void *params, clean_up_func_t clean_up)
{
	int IsEnqueued = 0;
	uid_t uid = UIDGenerate();
	task_t *new_task = NULL;
	assert(NULL != sd);
	assert(NULL != params);
	new_task = CreateTask(uid, exe_time, func, params, clean_up);
	if(NULL == new_task)
	{
		return bad_uid;
	}
	IsEnqueued = PQEnqueue(new_task);
	return IsEnqueued ? uid : bad_uid;
}

void SCHEDRemoveTask(my_uid_t task_id, sd_t *sd)
{
	assert(NULL != sd);
	PQErase(sd->pq, MatchUID, task_id)
}

int SCHEDIsEmpty(const sd_t *sd)
{
	assert(NULL != sd);
	return PQIsEmpty(sd->pq);
}

size_t SCHEDSize(const sd_t *sd)
{
	assert(NULL != sd);
	return PQSize(sd->pq);
}
void SCHEDClear(sd_t *sd)
{
	assert(NULL != sd);
	return PQClear(sd->pq);
}

sd_t *SCHEDCreate(compare_func_t priority_func)
{
	sd_t *new_scheduler = malloc(sizeof(sd_t));
	if(NULL == new_scheduler)
	{
		return NULL;
	}
	new_scheduler->pq = PQCreate(priority_func);
	if(NULL == new_scheduler->pq)
	{
		free(new_scheduler);
		return NULL;
	}
	new_scheduler->stop = 0;
	
	return new_scheduler;
}

void SCHEDDestroy(sd_t *sd)
{
	assert(NULL != sd);
	PQDestroy(sd->pq);
	free(sd);
}

static int MatchUID(my_uid_t uid1, my_uid_t uid2)
{
	return uid1==uid2;
}



