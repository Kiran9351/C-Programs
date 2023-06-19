// Array as queue.

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct queue
{
	int *arr;
	int size;
	int front;
	int top;
}Q;

Q *create_queue(int len)
{
	Q *q = NULL;
	q = (Q*) malloc(sizeof(Q));
	if(q == NULL)
	{
		return NULL;
	}

	q -> arr = (int*)malloc((len - 1) * sizeof(int));
	q -> size = len - 1;
	q -> front = -1;
	q -> top = -1;

	return q;
}

bool isFull(Q *q)
{
	if(q -> top == q -> size)
	{
		printf("Queue is full\n");
	}
	return (q -> top == q -> size);
}

bool isEmpty(Q *q)
{
	if(q -> front > q -> top || q -> front == -1)
	{
		printf("Queue is empty\n");
		q -> front = -1;
		q -> top = -1;
	}
	
	return (q -> front == -1);
}

void enque(Q* q, int data)
{
	if(isFull(q))
	{
		return;
	}

	if(q -> front == -1)
	{	
		q -> front += 1;
	}

	q -> top += 1;
	q -> arr[q -> top] = data;
}

int deque(Q *q)
{
	if(isEmpty(q))
	{
		return -1;
	}

	int val = q -> arr[q -> front];
	q -> front += 1;

	return val;
}

void printQueue(Q *q)
{
	if(isEmpty(q))
	{
		return; 
	}

	int i = 0;

	for(i = q -> front; i <= q -> top; ++i)
	{
		printf("%d ", q -> arr[i]);
	}

	printf("\n");
}

int main()
{
	Q *q = NULL;
	int len = 0;

	printf("Enter the size of queue : \n");
	scanf("%d",&len);

	q = create_queue(len);

	enque(q, 10);
	enque(q, 20);
	enque(q,30);
	enque(q, 40);
	
	printQueue(q);
	
	int val = deque(q);
	printf("%d\n",val);
	
	deque(q);
	deque(q);
		
	printQueue(q);
	enque(q,5);
	enque(q,8);
	enque(q,6);
	
	printQueue(q);	

	return 0;
}
