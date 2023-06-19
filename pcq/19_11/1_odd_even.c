

#include<stdio.h>
#include<stdlib.h>

void rearrange(int *arr, int n)
{
	int i = 0, j = 0, temp = 0, k = 0,cnt = 0;

	while(k < n && i < n)
	{
		while(arr[i] % 2 != 0 && i < n)
		{
			i++;
		}
		
		temp = arr[k];
		
		if(k % 2 == 0)
		{
			arr[k] = arr[i];
			arr[i] = temp;
		} 	
		
		k++;
	}

}

void rearrange_(int *arr, int n)
{
	int i = 0, j = 1, temp = 0, cnt = 0;

	while(i < n && j < n)
	{
		while(arr[i] % 2 == 0 && i < n)
		{
			i += 2;
		}

		while(arr[j] % 2 != 0 && j < n)
		{	
			j += 2;
		}

		temp = arr[i];
		arr[i] = arr[j];
		arr[j] = temp;
	}

	printf("%d\n",cnt);
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
	int n = 0, *arr = NULL, i = 0;

	printf("Enter the size of array : ");
	scanf("%d",&n);

	arr = (int*)malloc(n * sizeof(int));
	
	for(i = 0; i < n; ++i)
	{
		printf("Enter element at index %d : ", i);
		scanf("%d",(arr + i));
	}

	printarray(arr, n);
	rearrange_(arr, n);
	
	printarray(arr,n);	

	return 0;
}
