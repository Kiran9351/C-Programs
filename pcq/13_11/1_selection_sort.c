// Q1 Selection sort

#include<stdio.h>
#include<stdlib.h>


int *selection_sort(int *arr, int n)
{
	if(arr == NULL)
	{
		return NULL;
	}

	int i = 0, j = 0, temp = 0, min = 0, index = 0;

	for (i = 0; i < n; ++i)
	{
		min = arr[i];
		for(j = i; j < n; ++j)
		{
			if(arr[j] <= min)
			{
				index = j;
				min = arr[j];
			} 
		}
		
		temp = arr[i];
		arr[i] = min;
		arr[index] = temp;
	}

	return arr;
}

int *selection_sort_1(int *arr, int n)
{
	if(arr == NULL)
	{
		return NULL;
	}

	int i = 0, j = 0, temp = 0, max = 0, index = 0;

	for(i = n - 1; i >= 0; --i)
	{
		max = arr[i];

		for(j = 0; j <= i; ++j)
		{
			if(max <= arr[j])
			{
				index = j;
				max = arr[j];
			}
		}

		temp = arr[i];
		arr[i] = max;
		arr[index] = temp;
	}

	return arr;
}

void printarray(int *arr,int n)
{
	int i = 0;

	for(i = 0; i < n; ++i)
	{
		printf("%d ",arr[i]);
	}
	
	printf("\n");
}

int main()
{
	int *arr = NULL, n = 0, i = 0;

	printf("Enter size of array: ");
	scanf("%d",&n);

	arr = (int*)malloc(n * sizeof(int));

	printf("Enter elements of an array:\n");

	for(i = 0; i < n; ++i)
	{
		printf("Enter element at index %d : ",i);
		scanf("%d",(arr+i));
	}

	printarray(arr,n);

	//arr = selection_sort(arr,n);
	arr = selection_sort_1(arr,n);	
	printarray(arr,n);
	return 0;
}
