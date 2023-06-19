

#include<stdio.h>
#include<stdlib.h>

typedef struct node
{
	int data;
	struct node *next;
}node;

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

node *createLL()
{
	int n = 0, val = 0, i = 0;

	printf("Enter the size of LL : ");
	scanf("%d",&n);
	
	node *head = NULL, *prev = NULL, *newNode = NULL;

	for(i = 0; i < n; ++i)
	{
		printf("Enter data : ");	
		scanf("%d",&val);

		newNode = create_node(val);

		if(head != NULL)
		{
			prev -> next = newNode;
		}
		else
		{
			head = newNode;
		}

		prev = newNode;
	}

	return head;
}

int count(node *head)
{
	int cnt = 0;

	while(head != NULL)
	{
		cnt++;
		head = head -> next;
	}

	return cnt;
}

void printLL(node *head)
{
	while(head != NULL)
	
	{
		printf("|%d|->",head->data);
		head = head -> next;
	}

	printf("\n");
}

int main()
{
	node *head = NULL;
	head = createLL();
	
	int cnt = count(head);
	printf("%d\n",cnt);
	
	printLL(head);
	
	return 0;
}
