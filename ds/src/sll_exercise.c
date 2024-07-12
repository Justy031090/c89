


typedef struct node
{
	void *data;
	struct node *next;
} node_t;


node_t *Flip(node_t *head)
{
	node_t SecNode = head->next->next;
	node_t NextNode = head->next;
	// 1-2-3-4-5-6-X
	// T
	// T = &2
	// 1d
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
	// 1 2 3 4 5 6 7 X
	// 1 2 3         X
}
