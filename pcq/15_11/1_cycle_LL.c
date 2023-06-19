

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

struct node
{
	int data;
	struct node *next;
};

typedef struct node* Node;

struct node *create_node(int val)
{
	struct node* newNode;
	newNode =  malloc(sizeof(Node));
	
	if(newNode == NULL)
	{
		return NULL;
	}
	
	newNode -> data = val;
	newNode -> next = NULL;

	return newNode;
}

struct node *create_LL()
{
	int n = 0, i = 0, val = 0;

	Node head = NULL, prev = NULL, newNode = NULL;

	printf("Enter size of LL : ");
	scanf("%d",&n);

	for( i = 0; i < n; ++i)
	{
		printf("Enter data : ");	
		scanf("%d", &val);
		newNode = create_node(val);		
		
		if(head != NULL)
		{
			prev->next  = newNode;
		}	
		else
		{
			head = newNode;
		}
		prev = newNode;
	}
	
	return head;
}

void printLL(Node head)
{
	if(head == NULL)
	{
		return;
	}

	while(head != NULL)
	{
		printf("|%d|->",head->data);
		head = head -> next;
	}

	printf("\n");
	
	return;
}

bool cycle(Node head)
{
	if(head == NULL)
	{
		return false;
	}

	Node slow = head , fast = head->next;

	while(slow != fast && fast != NULL)
	{
		slow = slow->next;
		fast = fast->next;
		if(fast == NULL)
		{
			return false;
		}
		fast = fast->next;
	}
 
	if(slow == fast)
	{
		return true;
	}
	else
	{
		return false;
	}
}

Node create_cycle(Node head)
{
	if(head == NULL)
	{
		return NULL;
	}
	int i = 0;
	Node newHead = head;
	Node temp = head;
	while(head->next != NULL)
	{
		head = head-> next;
	}

	for(i = 0; i < 4; ++i)
	{
		temp = temp->next;
	}

	head->next = temp;

	return newHead;
}

int main()
{
	struct node *head = NULL;
	bool bRet = false;

	head = create_LL();
	printLL(head);
	head = create_cycle(head);
	bRet = cycle(head);
	if(bRet == true)
	{
		printf("Cycle is there in LL\n");
	}
	else
	{
		printf("Cycle is not there in LL\n");
	}

	return 0;
}
