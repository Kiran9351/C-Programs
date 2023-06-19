

#include<stdio.h>
#include<stdlib.h>

typedef struct node
{
	int data;
	struct node *next;
	struct node *prev;
}node;

node *createnode(int val)
{
	node *newNode = NULL;

	newNode = (node*)malloc(sizeof(node));

	if(newNode == NULL)
	{
		return NULL;
	}

	newNode -> data = val;
	newNode -> next = NULL;
	newNode -> prev = NULL;

	return newNode;
}

node *createDLL()
{
	int n = 0, i = 0, val = 0;
	
	printf("Enter size of DLL : ");
	scanf("%d",&n);

	node *head = NULL, *curr = NULL, *newNode = NULL;

	for(i = 0; i < n ; ++i)
	{
		printf("Enter data : ");
		scanf("%d",&val);
		
		newNode = createnode(val);

		if(head != NULL)
		{
			curr -> next = newNode;
			newNode -> prev = curr;
		}
		else
		{
			head = newNode;
			newNode -> prev = NULL;
		}
		curr = newNode;
	}

	return head;
}

void printDLL(node *head)
{
	while(head != NULL)
	{
		printf("<-|%d|->",head -> data);
		head = head -> next;
	}

	printf("\n");
}

node *gototail(node *head)
{
	while(head -> next != NULL)
	{
		head = head -> next;
	}

	return head;
}

void reversedisplay(node *head)
{
	head = gototail(head);

	while(head != NULL)
	{
		printf("<-|%d|->",head -> data);
		head = head -> prev;
	}

	printf("\n");
}

int main()
{
	struct node *head = NULL;

	head = createDLL();
	printDLL(head);
	reversedisplay(head);
	return 0;
}
