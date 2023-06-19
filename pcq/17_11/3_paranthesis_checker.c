

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct node
{
	char ch;
	struct node *next;
}node;

typedef struct stack
{
	node *head;
}S;

node *create_node(char ch)
{
	node* newNode = NULL;

	newNode = (node*)malloc(sizeof(node));

	if(newNode == NULL)
	{
		return NULL;
	}

	newNode -> ch = ch;
	newNode -> next = NULL;

	return newNode;
}

S *createstack()
{
	S *s = NULL;
	s = (S*)malloc(sizeof(S));

	s -> head = NULL;

	return s;
}

bool isEmpty(S*s)
{
	return (s->head == NULL);
}

void push(S *s, char ch)
{
	node *newNode = create_node(ch);
	
	newNode -> next = s -> head;
	s -> head = newNode;
}

char pop(S*s)
{
	if(isEmpty(s))
	{
		return '\0';
	}
	char ch = s -> head -> ch;
	s -> head = s -> head -> next;
	return ch;
}

void printStack(S*s)
{	
	node *temp = s -> head;
	
	while(temp != NULL)
	{
		printf("%c ",temp -> ch);
		temp = temp -> next;
	}

	printf("\n");
}

bool parenthesis_chk(S*s, char *str)
{
	if(str == NULL)
	{
		return true;
	}

	int i = 0;
	char ch = '\0';

	while(str[i] != '\0')
	{
		if(str[i] == '(' || str[i] == '[' || str[i] == '{')
		{
			push(s,str[i]);	
		}
		else if(str[i] == ')' || str[i] == ']' || str[i] == '}')
		{
			ch = pop(s);
		
			if(str[i] == ')' && ch != '(')
			{
				break;
			}
			else if(str[i] == ']' && ch != '[')
			{
				break;
			}
			else if(str[i] == '}' && ch != '{')
			{
				break;
			}
		}
		
		
		i++;
	}

	if(s -> head == NULL)
	{
		return true;
	}
	
	return false;
}

int main()
{
	bool bRet = false;

	S *s = NULL;

	s = createstack();

	char str[100];

	printf("Enter expression : ");
	scanf(" %[^\n]s",str);

	bRet = parenthesis_chk(s,str);
	if(bRet == true)
	{
		printf("Parenthesis are correct\n");
	}
	else
	{
		printf("Parenthesis are not correct\n");
	}
	
	return 0;
}
