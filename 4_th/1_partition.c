
#include<stdio.h>
#include<stdlib.h>

void swap(int *arr, int i, int p)
{
	int temp = arr[i];
	arr[i] = arr[p];
	arr[p] = temp;

	return;
}

int partition(int *arr , int n, int k)
{
	int i = 0, p = 0, j = 0;

	while(p < n)
	{
		if(arr[p] > k)
		{
			p++;
		}

		if(arr[i] <= k)
		{
			i++;
		}

		if(p < n && (arr[i] > k) && (arr[p] <= k))
		{
			swap(arr,i,p);
		}

		j = i;
	}

	return j;
}

void print_array(int *arr, int n)
{
	int i = 0;

	printf("Array is : ");
	for(i = 0 ; i < n; i++)
	{
		printf("%d ",arr[i]);
	}

	printf("\n");

	return;
}

int main()
{	
	int n = 0, *arr = NULL, i = 0, k = 0, j = 0;

	printf("Enter the size of array: ");
	scanf("%d",&n);

	arr = (int*)malloc(sizeof(int) * n);

	printf("Enter elements of array:\n");
	for(i = 0; i < n; i++)
	{
		printf("Enter enter element at index %d : ", i);
		scanf("%d",(arr + i));
	}

	printf("Enter value of k: ");
	scanf("%d",&k);

	j = partition(arr,n,k);
	printf("Partitioned index is : %d\n",j);

	print_array(arr,n);	

	return 0;
}
