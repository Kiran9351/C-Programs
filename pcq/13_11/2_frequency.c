
// Frequency of elements

#include<stdio.h>
#include<stdlib.h>

int frequency(int *arr, int n, int val)
{
	if(arr == NULL)
	{
		return -1;
	}

	int i = 0, cnt = 0;
	
	for(i = 0; i < n; ++i)
	{
		if(arr[i] == val)
		{
			cnt++;
		}
	}

	return cnt;
}

int main()
{
	int n = 0, *arr = NULL, i = 0, val = 0, ret = 0;
	
	printf("Enter size of array: ");
	scanf("%d",&n);
	
	arr = (int*)malloc(sizeof(int) * n);

	printf("Enter elements of array:\n");

	for(i = 0; i < n; ++i)
	{
		printf("Enter element at index %d : ", i);
		scanf("%d",(arr + i));
	}
	
	printf("Enter element to find freq: ");
	scanf("%d",&val);
	
	ret = frequency(arr,n, val);
	printf("Frequency of %d is : %d", val,ret);
	
	return 0;
}
