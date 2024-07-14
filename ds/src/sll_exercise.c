


typedef struct node
{
	void *data;
	struct node *next;
} node_t;


node_t *Flip(node_t *head)
{
	node_t prev = NULL;
	node_t next = NULL
	node_t current = head;
	
	while(NULL != current)
	{
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}
	head = prev;
}


int HasLoop(const node_t *head)
{
	node_t ptr1 = head->next;
	node_t ptr2 = head->z->next;
	
	while(NULL != ptr1)
	{
		if(ptr1->next == ptr2->next)
			return 1;
		
		ptr1 = ptr1->next;
		ptr2 = ptr2->next;
	}
	return 0; 
}


node_t *FindIntersection(node_t *head_1, node_t *head_2)
{
	int len1 = 0;
	int len2 = 0;
	node_t *counter1 = head1;
	node_t *counter2 = head2;
	
	while(counter1 != NULL)
	{
		counter1 = counter1->next;
		++len1;
	}
	
	while(counter2 != NULL)
	{
		counter2 = counter2->next;
		++len2;
	}
	
	len1 = len1-len2;
	if(len1 < len2)
	{
		while(len1 > 0)
		{
			head_1 = head_1->next;
			--len1;
		}
	}
	else
	{
		while(len2>0)
		{
			head_2 = head_2->next;
			--len2;
		}
	}
	
	while(head_1 != NULL)
	{
		if(head_1 == head_2)
		{
			return head_1;
		}
		head_1 = head_1->next;
		head_2 = head_2->next;
	}
	
	return NULL;

}
		
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}
