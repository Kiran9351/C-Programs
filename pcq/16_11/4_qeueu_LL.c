

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct node 
{
	int data;
	struct node *next;
}node;

typedef struct queue
{
	int len;
	node *head;
	node *tail;
}Q;

node *create_node(int val)
{
	node *newNode = NULL;

	newNode = (node*)malloc(sizeof(node));

	if(newNode == NULL)
	{
		return NULL;
	}

	newNode -> data = val;
	newNode -> next = NULL;

	return newNode;
}

Q *create_queue()
{
	Q *q = NULL;

	q = (Q*)malloc(sizeof(Q));
	
	if(q == NULL)
	{
		return NULL;
	}

	q -> len = 0;
	q -> head = NULL;
	q -> tail = NULL;

	return q;
}

bool isEmpty(Q*q)
{
	if(q -> head == NULL)
	{
		q -> tail = NULL;
	}

	return (q -> head == NULL);
}

void enque(Q *q, int val)
{
	node *newNode = create_node(val);

	if(q -> head != NULL)
	{
		q -> tail -> next = newNode;
	}
	else
	{
		q -> head = newNode;
	}

	q -> tail = newNode;
	q -> len += 1;
}

int deque(Q *q)
{
	if(isEmpty(q))
	{
		return -1;
	}

	int val = q -> head -> data;
	q -> head = q -> head -> next;
	
	q -> len -= 1;

	return val;
}

void printQueue(Q *q)
{
	node *temp = q -> head;

	while(temp != NULL)
	{
		printf("%d ",temp -> data);
		temp = temp -> next;
	}

	printf("\n");
}

int main()
{
	Q *q = NULL;

	q = create_queue();
	
	enque(q,10);
	enque(q,20);
	enque(q,30);

	printQueue(q);

	printf("%d\n",q -> len);
	int val = deque(q);
	printf("%d\n",val);

	printQueue(q);
	printf("%d\n",q -> len);

	return 0;
}
