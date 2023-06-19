

#include<stdio.h>
#include<stdlib.h>

int kth_large(int *arr, int n, int k)
{
	if(arr == NULL)
	{
		return -1;
	}

	int i = 0, j = 0, max = 0, temp = 0, index = 0;

	for(i = n - 1; i >= n - k; --i)
	{
		max = arr[i];
		index = i;
		for(j = 0; j < i; ++j)
		{
			if(arr[j] > max)
			{
				max = arr[j];
				index = j;
			}
		}
		temp = arr[i];
		arr[i] = max;
		arr[index] = temp;
		
	}  

	return arr[++i];
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

	ret = kth_large(arr,n,k);
	printf("kth largest element is : %d\n",ret);
	
	return 0;
}
