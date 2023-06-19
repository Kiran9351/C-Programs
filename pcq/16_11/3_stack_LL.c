

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct node
{
	int data;
	struct node *next;
}node;

typedef struct stack
{
	int len;
	node *head;
}S;

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

S *create_stack()
{
	S *s = NULL;
	s = (S*)malloc(sizeof(S));
	if(s == NULL)
	{
		return NULL;
	}

	s -> len = 0;
	s -> head = NULL;

	return s;
}

bool isEmpty(S*s)
{
	if(s -> head == NULL)
	{
		printf("Stack is empty\n");
	}
	
	return (s -> head == NULL);
}

void push(S *s, int val)
{
	node* newNode = create_node(val);

	if(newNode == NULL)
	{
		return;
	}

	if(s -> head != NULL)
	{
		newNode -> next = s -> head;
	}
	
	s -> head = newNode;
	s -> len += 1;
}

int pop(S *s)
{
	if(isEmpty(s))
	{
		return -1;
	}

	int val = s -> head -> data;

	node *temp = s -> head;
	s -> head = s -> head -> next;

	s -> len -= 1;
	
	return val;
}

void popa(S*s)
{
	while(s -> head != NULL)
	{
		node *temp = s -> head;
		s -> head = temp -> next;
		printf("%d ",temp -> data);
	}
	
	printf("\n");
}

int top(S *s)
{
	if(s != NULL && s -> head != NULL)
	{
		return s -> head -> data;
	}

	return -1;
}

void printStack(S *s)
{
	if(isEmpty(s))
	{
		return;
	}

	node *temp = s -> head;
		
	while(temp != NULL)
	{
		printf("%d ",temp -> data);
		temp = temp -> next;
	}

	printf("\n");
}

int count(S *s)
{
	return s -> len;
}

int main()
{
	S *s = NULL;
	s = create_stack();

	push(s,10);
	push(s,25);
	push(s,85);
	push(s,65);
	int val = count(s);
	printf("%d\n",val);
	printStack(s);
	
	val = pop(s);
	printf("%d\n",val);

	popa(s);
	val = count(s);
	printf("%d\n",val);

	printStack(s);
	
	return 0;
}
