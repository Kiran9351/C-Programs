
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct node
{
	int data;
	struct node *next;
}node;

node* create_node(int data)
{
	node *newNode = NULL;

	newNode = (node*)malloc(sizeof(node));
	
	if(newNode == NULL)
	{
		return NULL;
	}

	newNode -> data = data;	
	newNode -> next = NULL;

	return newNode;
}

node *createLL()
{
	int n = 0, i = 0, val = 0;

	printf("Enter size of LL : ");
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

node *create_loop(node *head)
{
	if(head == NULL)
	{
		return NULL;
	}
	node *temp = head;

	while(temp -> next != NULL)
	{
		temp = temp -> next;
	}

	temp -> next = head -> next ->next;

	return head;
}

bool detectloop(node *head)
{
	if(head == NULL)
	{
		return false;
	}

	node *slow = head, *fast = head->next;

	while(slow != fast && fast != NULL)
	{
		fast = fast -> next;
		if(fast == NULL)
		{
			break;
		}
		fast = fast -> next;
		slow = slow -> next;
	}

	if(slow == fast)
	{
		return true;
	}
	
	return false;
}

void printLL(node *head)
{
	while(head != NULL)
	{
		printf("|%d|->",head -> data);
		head = head -> next;
	}

	printf("\n");
}

int main()
{
	bool bRet = false;

	node *head = NULL;
	head = createLL();
	printLL(head);
	head = create_loop(head);
	bRet = detectloop(head);
	if(bRet == true)
	{
		printf("There is loop\n");
	}	
	else
	{
		printf("There is not loop\n");
	}
	
	return 0;
}
