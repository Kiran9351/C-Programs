

#include<stdio.h>
#include<stdlib.h>

struct node 
{
	int data;
	struct node *next;
};

struct node *create_node(int val)
{
	struct node *newNode = (struct node*)malloc(sizeof(struct node));

	if(newNode != NULL)
	{
		newNode -> data = val;
		newNode -> next = NULL;
	}

	return newNode;
}

struct node* createLL()
{
	int n = 0, i = 0, val = 0;
	
	printf("Enter size of LL : ");
	scanf("%d",&n);

	struct node *head = NULL, *prev = NULL, *newNode = NULL;

	for( i = 0; i < n; ++i)
	{
		printf("Enter data: ");
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

void printLL(struct node *head)
{
	while(head != NULL)
	{
		printf("|%d|->",head->data);
		head = head -> next;
	}
	printf("\n");
}

struct node* deep_copy(struct node *head)
{
	struct node *deephead = NULL, *prev = NULL;

	while(head != NULL)
	{
		if(deephead != NULL)
		{
			prev -> next = create_node(head->data);
			prev = prev->next;
		}	
		else
		{
			deephead = create_node(head->data);
			prev = deephead;
		}

		head = head->next;
	}

	return deephead;
}

int main()
{
	struct node *head = NULL;

	head = createLL();
	printLL(head);

	struct node *deephead = deep_copy(head);

	printLL(deephead);
	return 0;
}
