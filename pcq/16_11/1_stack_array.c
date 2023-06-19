

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct stack
{
	int *arr;
	int size;
	int top;
}S;

S *create_stack(int len)
{
	S *s = NULL;

	s = (S*)malloc(sizeof(S));
	if(s == NULL)
	{
		return NULL;
	}

	s -> arr = (int*)malloc(len * sizeof(int));
	s -> size = len;
	s -> top = -1;

	return s;
}

void push(S *s, int data)
{
	if(s -> top == s->size)
	{
		printf("Stack is full\n");
		return;
	}

	s -> top += 1;
	s -> arr[s->top] = data;
}

int pop(S *s)
{
	if(s -> top == -1)
	{
		printf("Stack is empty\n");
		return -1;
	}

	int val = s -> arr[s -> top];
	s -> top -= 1;

	return val;
}

bool isFull(S *s)
{
	return (s -> top == s->size);
}

bool isEmpty(S *s)
{
	return (s -> top == -1);
}

int TOP(S *s)
{
	if(isEmpty(s))
	{
		return -1;
	}
	
	return s -> arr[s -> top];
}

void printStack(S*s)
{
	if(isEmpty(s))
	{
		return;
	}

	int i = 0;
	for(i = 0; i < s -> top + 1; ++i)
	{
		printf("%d ",s->arr[i]);
	}
	
	printf("\n");
}

int main()
{
	S *s = NULL;
	int len = 0;

	printf("Enter size of stack: ");
	scanf("%d",&len);

	s = create_stack(len);
	
	push(s,5);
	push(s,25);
	push(s,35);
	push(s,45);
	push(s,65);
	
	printStack(s);
		
	int val = TOP(s);
	printf("%d\n",val);
	
	val = pop(s);
	printf("%d\n",val);

	val = TOP(s);
	printf("%d\n",val);

	printStack(s);	

	return 0;
}
