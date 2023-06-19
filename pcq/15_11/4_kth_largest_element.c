

#include<stdio.h>
#include<stdlib.h>

typedef struct queue
{
	int *brr;
	int front;
	int top;
}Q;


int deque(Q* q, int k)
{
  	if(q == NULL)
        {
        	return -1;
        }
                     
        int val = q -> brr[q -> front];
	
	q -> brr[q -> front];
 	q -> front = (q -> front + 1) % k;
	
	return val;
}

void enque(Q* q,int data, int k)
{
	if(q == NULL)
  	{
  		return;
  	}
  	
 	int val = deque(q,k);
	
	if(q -> front == -1)
	{
		q -> front = 1;	
	}
	
	q -> top++;
	q -> brr[q -> top % k] = data;
}

int kth_large(int *arr, int n, int k, Q* q)
{
	if(arr == NULL)
	{
		return -1;
	}

	int i = 0, max = -1;

	enque(q,arr[0],k);

	for(i = 1; i < n; ++i)
	{
		if(max < arr[i])
		{
			max = arr[i];
			enque(q,max,k);
		}
	}	

	return q -> brr[q -> front];
}

int main()
{
	int n = 0, i = 0, *arr = NULL, ret = 0, k = 0;
	
	printf("Enter the size of an array: ");
	scanf("%d",&n);

	arr = (int*)malloc(n * sizeof(int));

	for(i = 0; i < n; ++i)
	{
		printf("Enter element at index %d : ",i);
		scanf("%d",(arr + i));
	}

	printf("Entet value of k: ");
	scanf("%d",&k);

	struct queue* q = (Q*) malloc(sizeof(Q));
	q -> brr = (int*)malloc(k * sizeof(int));
	q -> top = -1;
	q -> front = -1;	

	ret = kth_large(arr,n,k,q);
	printf("kth largest element is : %d\n",ret);
	
	return 0;
}
