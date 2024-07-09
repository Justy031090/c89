




typedef struct node
{
	void *data;
	node_t *next;
}node_t;

typedef struct sll
{
	node_t *head;
	node_t *tail;
} sll_t;


sll_t *SLLCreate(void)
{
	sll_t new_sll = malloc(sizeof(sll));
	node_t dummy = malloc(sizeof(node));
	if(NULL == new_sll || NULL == dummy)
	{
		return NULL;
	}
	
	dummy->next = NULL;
	dummy->data = NULL;
	new_sll->head = dummy;
	new_sll->tail = dummy;
	
	return new_sll;
}

size_t SLLSize(const sll_t *sll)
{
	size_t size = 0;
	node_t *counter = sll->head;
	while(NULL != start->next)
	{
		counter = counter->next;
		++size;
	}
	
	return size;
}

int SLLIsEmpty(const sll_t *sll)
{
	return sll->head == sll->tail ? 1 : 0;
}

sll_iterator_t *SLLEnd(const sll_t *sll)
{
	node_t *end_node = sll->head;
	while(NULL != start->next)
	{
		end_node = end_node->next;
	}
	/*should return the iterator*/
	return end_node;
}

sll_iterator_t *SLLBegin(const sll_t *sll)
{
	
	return sll->head;
}

void *SLLGetData(const sll_iterator_t *iter)
{
	return iter->data;
}

void SLLSetData(sll_iterator_t *iter, const void *data)
{
	iter->data = data;
}

sll_iterator_t *SLLNext(sll_iterator_t *iter)
{
	return iter->next;
}
int SLLIsEqual(const sll_iterator_t *iter1, const sll_iterator_t *iter2)
{
	return iter1->data == iter2->data ? 1:0;
}

sll_iterator_t *SLLFind(const sll_iterator_t *from, const sll_iterator_t *to, is_match_t is_match, void *param)
{
	sll_iterator_t *next = from;
	while(next != to)
	{
		if(is_match(param) == next->data)
		{
			return next;
		}
		next = next->next;
	}
	return 
}

status_t SLLInsert(sll_iterator_t *iter, const void *data)
{
	sll_iterator_t new_node = malloc(sizeof(node));
	if(NULL == new_node)
	{
		return MEM_FAIL;
	}
	if(NULL == iter->next)
	{
		sll->head = iter;
	}
	else
	{
		new_node->next = iter->next;
	}
	new_node->data = data;
	iter->next = new_node;
	
	return SUCCESS;
}

sll_iterator_t *SLLRemove(sll_iterator_t *iter)
{
	sll_iterator tmp = iter->next;
	iter->data = tmp->data;
	iter->next = tmp->next;
	free(tmp);
	tmp = NULL;
	return iter;
}




void SLLDestroy(sll_t *sll)
{	
	/**********CHECK****************/
	sll_iterator_t start = sll->head;
	while(NULL != start->next)
	{
		free(start->next);
	/**********CHECK****************/
	}
	free(sll);
	sll = NULL;
	
}



void *SLLForEach(const sll_iterator_t *from, const sll_iterator_t *to, action_t *action_func, void *param);

